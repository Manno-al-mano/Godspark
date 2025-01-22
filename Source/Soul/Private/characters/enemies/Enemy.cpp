// Fill out your copyright notice in the Description page of Project Settings.


#include "characters/enemies/Enemy.h"
#include "AIController.h"
#include "MyComponents/AttributeComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Items/MyWeapon.h"
#include "Items/SoulItem.h"
#include "Navigation/PathFollowingComponent.h"
#include "Items/Projectile.h"
// Sets default values
AEnemy::AEnemy():Super()
{

	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	HealthBarComponent = CreateDefaultSubobject<UHealthBarComponent>(TEXT("Healthbar"));
	HealthBarComponent->SetupAttachment(GetRootComponent());
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AEnemy::ShootProjectile()
{
	UWorld* World = GetWorld();
	if (World && ProjectileClass&&CombatTarget) {
		
		FVector SocketLocation = GetMesh()->GetSocketLocation(RangedSocketName);
		FRotator SocketRotation = GetMesh()->GetSocketRotation(SocketName);
		AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, SocketLocation, SocketRotation);
		if (Projectile)
		{
			Projectile->SetOwner(this);
			Projectile->SetInstigator(this);
			Projectile->SetTarget(CombatTarget);
		}
	}
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (PawnSensor) {
		PawnSensor->OnHearNoise.AddDynamic(this, &AEnemy::HearNoise);
	}
	Tags.Add(FName("EnemyFaction"));
	if (HealthBarComponent&&Attribute&&HealthBarComponent->GetWidgetClass())
	{
		ChangeHealthbarVisibility(false);
		HealthBarComponent->SetName(InGameName);
		HealthBarComponent->SetHealthPercent(GetCurrentHealth());
		HealthBarComponent->SetPoisePercent(GetCurrentPoise());
	}
	EnemyController = Cast<AAIController>(GetController());
	StartMovement(PatrolTargetCurrent);
	UWorld* World = GetWorld();
	if (World && WeaponClass) {
		AMyWeapon* Weapon = World->SpawnActor<AMyWeapon>(WeaponClass);
		Weapon->Equip(GetMesh(), FName("MainHandSocket"), this, this);
		EquippedWeapon = Weapon;
		HealthBarComponent->SetTextColor(NameColor);
	}
}

void AEnemy::Die_Implementation()
{
	ClearAttackTimer();
	ClearPatrolTimer();
	Super::Die_Implementation();
	SetLifeSpan(10);
	UWorld* World = GetWorld();
	SpawnSoul(World);
	EnemyState =EEnemyState::EES_Dead;
	if (EquippedWeapon)EquippedWeapon->SetLifeSpan(10);
	ChangeHealthbarVisibility(false);
}

void AEnemy::SpawnSoul(UWorld* World)
{
	if (World && SoulClass) {
		const FVector SpawnLocation = GetActorLocation() + FVector(0.f, 0.f, 125.f);
		ASoulItem* Soul = World->SpawnActor<ASoulItem>(SoulClass, SpawnLocation, GetActorRotation());
		if (Attribute&&Soul) 
		{
			Soul->SetSoulCount(Attribute->GetSoulCount());
			Soul->SetOwner(this);
		}
	}
}

void AEnemy::StartMovement(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr) return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(50.f);
	EnemyController->MoveTo(MoveRequest);
}

void AEnemy::PatrolTargetFinished()
{
	StartMovement(PatrolTargetCurrent);
}

void AEnemy::HandleDamage(float Damage)
{
	Super::HandleDamage(Damage);
	if (Attribute && HealthBarComponent) {
		{
			HealthBarComponent->SetHealthPercent(Attribute->CalculateHealth());
			HealthBarComponent->SetPoisePercent(Attribute->CalculatePoise());
		}
	}


}

void AEnemy::RegenPoise()
{
	if (Attribute && HealthBarComponent) {
		Attribute->IncreasePoise();
		float PoisePercent = Attribute->CalculatePoise();
		HealthBarComponent->SetPoisePercent(PoisePercent);
		if (PoisePercent != 0)
			GetWorldTimerManager().SetTimer(PoiseRegen, this, &AEnemy::RegenPoise, LaterRegenTime);

	}
}

void AEnemy::ChangeHealthbarVisibility(bool status)
{
	if (HealthBarComponent)
		HealthBarComponent->SetVisibility(status);
}

void AEnemy::LoseTarget()
{
	ChangeHealthbarVisibility(false);
	CombatTarget = nullptr;
	EnemyState = EEnemyState::EES_Patrolling;

}

void AEnemy::GoOnPatrol()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	StartMovement(PatrolTargetCurrent);
}

bool AEnemy::IsOutsideCombatRange()
{
	if (CombatTarget == nullptr) return true;
	return !InTargetRange(CombatTarget, CombatRadius);
}

bool AEnemy::IsOutsideAttackRadius()
{
	return !InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsDead()
{
	return EnemyState == EEnemyState::EES_Dead;
}

void AEnemy::StartChasing()
{

	EnemyState = EEnemyState::EES_Chasing;
	ClearPatrolTimer();
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	StartMovement(CombatTarget);
	ChangeHealthbarVisibility(true);
}

void AEnemy::StartAttackTimer()
{
	EnemyState = EEnemyState::EES_Engaged;
	const float AttackTime = FMath::RandRange(AttackCooldownMin, AttackCooldownMax);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
}

void AEnemy::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
}

void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
	AttackIterator = 0;
}

void AEnemy::AttackEnd()
{
	EnemyState = EEnemyState::EES_Stateless;
	EnemySearch();
}

void AEnemy::ChooseHitSide(const FVector& ImpactPoint)
{
	if (EnemyState == EEnemyState::EES_Dead) return;
	ClearAttackTimer();
	SetWeaponCollision(ECollisionEnabled::NoCollision);
	Super::ChooseHitSide(ImpactPoint);
	EnemyState = EEnemyState::EES_Staggered;
}

void AEnemy::PawnSeen(APawn* SeenPawn)
{
	const bool bShouldChase = EnemyState < EEnemyState::EES_Chasing
		&& !IsDead()
		&& SeenPawn->ActorHasTag(FName("PlayerFaction"))
		&& !SeenPawn->ActorHasTag(FName("Dead"));
	if (bShouldChase) {
		CombatTarget = SeenPawn;
		StartChasing();

	}
}

void AEnemy::EndStagger()
{
	if (EnemyState == EEnemyState::EES_Dead) return;
	EnemyState = EEnemyState::EES_Stateless;
	EnemySearch();
}

void AEnemy::HearNoise(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	PawnSeen(NoiseInstigator);
}

void AEnemy::Attack()
{
	if(CombatTarget->ActorHasTag(FName("Dead")))
	{
		CombatTarget = nullptr;
		return;
	}
	if (AttackMontageSections.Num() == 0) return;
	EnemyState = EEnemyState::EES_Attacking;
	if (!InTargetRange(CombatTarget, CloseAttackRadius)) {
		JumpAttack();
		return;
	}
	switch (bDoesChainAttacks)
	{
	case true:
		OrderedAttack();
		break;
	case false:
		RandomAttack();
	}


}

void AEnemy::JumpAttack()
{
	if(RangedMontageSections.Num()>0)
	{
		int32 Section = FMath::RandRange(0, RangedMontageSections.Num() - 1);
		PlayAttackMontage(RangedMontageSections[Section], RangedAttackSpeed);
	}
	else
	PlayAttackMontage("JumpAttack", AttackMontageSpeed);
}

void AEnemy::EnemySearch() {
	//
	if (IsOutsideCombatRange() && EnemyState!=EEnemyState::EES_Patrolling) {
		LoseTarget();
		if (EnemyState <EEnemyState::EES_Attacking)GoOnPatrol();
		ClearAttackTimer();
	}
	if (CombatTarget == nullptr) return;
	else if (IsOutsideAttackRadius() && EnemyState != EEnemyState::EES_Chasing) {
		if (EnemyState <EEnemyState::EES_Attacking)StartChasing();
		ClearAttackTimer();
	}
	else if (!IsOutsideAttackRadius() && EnemyState < EEnemyState::EES_Engaged) {
		StartAttackTimer();
	}
}

AActor* AEnemy::PatrolArea() {
	TArray<AActor*> ValidTargets;
	for (auto Target : PatrolTargets) {
		if (Target != PatrolTargetCurrent)
			ValidTargets.Add(Target);
	}
	const int32 NumberOfTargets = ValidTargets.Num();
	if (NumberOfTargets > 0)
	{
		const int32 NextTarget = FMath::RandRange(0, NumberOfTargets - 1);
		return ValidTargets[NextTarget];

	}
	return PatrolTargetCurrent;
}

void AEnemy::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);
	if (IsDead()) return;
	EnemySearch();
	if (InTargetRange(PatrolTargetCurrent, PatrolRadius) && EnemyState == EEnemyState::EES_Patrolling) {
		PatrolTargetCurrent = PatrolArea();
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTargetFinished, FMath::FRandRange(WaitMin, WaitMax));
	}
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint, const FVector& ImpactNormal, const FVector& ActorLocation)
{
	ChangeHealthbarVisibility(true);
	Super::GetHit_Implementation(ImpactPoint, ImpactNormal, ActorLocation);
	ClearPatrolTimer();
	

}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if(!CombatTarget)
	{
		CombatTarget = EventInstigator->GetPawn();
		StartChasing();
	}
	GetWorldTimerManager().SetTimer(PoiseRegen, this, &AEnemy::RegenPoise, AfterHitRegenTime);
	return DamageAmount;
}

