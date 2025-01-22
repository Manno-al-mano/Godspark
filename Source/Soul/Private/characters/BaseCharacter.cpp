// Fill out your copyright notice in the Description page of Project Settings.


#include "characters/BaseCharacter.h"
//#include "characters/CharacterStates.h"
#include "Items/MyWeapon.h"
#include "Components/BoxComponent.h"
#include "Perception/PawnSensingComponent.h"
//#include "Animation/AnimMontage.h"
#include "MyComponents/AttributeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetGenerateOverlapEvents(true);
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Attribute = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	PawnSensor = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensor"));
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (PawnSensor) {
		PawnSensor->OnSeePawn.AddDynamic(this, &ABaseCharacter::PawnSeen);
	}
	
}

void ABaseCharacter::PlayAttackMontage(const FName& AttackName, float MontageSpeed)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage&& EquippedWeapon) {
		AnimInstance->Montage_Play(AttackMontage, MontageSpeed*EquippedWeapon->GetAttackSpeed());
		AnimInstance->Montage_JumpToSection(AttackName);
	}
}

void ABaseCharacter::AttackEnd()
{
}
void ABaseCharacter::HandleDamage(float Damage)
{
	if (Attribute) {
		if (Attribute->CalculatePoise() == 1.f)
			Damage *= PoiseBreakMultiplier;
		Attribute->DamageHealth(Damage);
		Attribute->DamagePoise();
	}

}
bool ABaseCharacter::isAlive()
{
	return Attribute && Attribute->IsAlive();
}

void ABaseCharacter::RandomAttack()
{
	int32 Section = FMath::RandRange(0, AttackMontageSections.Num() - 1);
	PlayAttackMontage(AttackMontageSections[Section], AttackMontageSpeed);
}

void ABaseCharacter::OrderedAttack()
{
	MakeNoise(1.f, this, GetActorLocation());
	PlayAttackMontage(AttackMontageSections[AttackIterator], AttackMontageSpeed);
	AttackIterator = AttackIterator == AttackMontageSections.Num()-1 ? 0 : AttackIterator + 1;
}

void ABaseCharacter::ResetAttackIterator()
{
	AttackIterator = 0;
}

float ABaseCharacter::GetCurrentStamina()
{
	if (Attribute)
	return	Attribute->CalculateStamina();
		return 0.f;
}

float ABaseCharacter::GetCurrentHealth()
{
	if (Attribute)
	return	Attribute->CalculateHealth();
	return 0.f;
}

float ABaseCharacter::GetCurrentPoise()
{
	if (Attribute)
	return	Attribute->CalculatePoise();
	return 0.f;
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint, const FVector& ImpactNormal, const FVector& ActorLocation)
{
	if (HitSound)
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint);
	if (HitParticles) {
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticles,
			ImpactPoint,
			FRotationMatrix::MakeFromZ(ImpactNormal).Rotator(),
			(FVector)(ParticleScale));
	}
	if (isAlive())
	{
		if (Attribute->CalculatePoise() == 0.f) {
			ChooseHitSide(ActorLocation);
		}
		if (Attribute->CalculatePoise() == 1.f)
		{
			if (BreakSound)
			{
				UGameplayStatics::PlaySoundAtLocation(
					this,
					BreakSound,
					ImpactPoint);
			}
		}
	}
	else Die();
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	return DamageAmount;
}

void ABaseCharacter::SetWeaponCollision(ECollisionEnabled::Type CollisionType)
{
	if (EquippedWeapon && EquippedWeapon->GetBoxComponent()) {
		EquippedWeapon->GetBoxComponent()->SetCollisionEnabled(CollisionType);
		if (CollisionType == ECollisionEnabled::NoCollision) {
			EquippedWeapon->IgnoreActors.Empty();
		}
	}
}

double ABaseCharacter::CalculateHitAngle(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();
	const FVector LoweredImpactPoint = FVector(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (LoweredImpactPoint - GetActorLocation()).GetSafeNormal();
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0) {
		Theta *= -1;
	}
	return Theta;
}

void ABaseCharacter::ChooseHitSide(const FVector& ImpactPoint)
{
	double Theta = CalculateHitAngle(ImpactPoint);
	if (Theta > 45 && Theta < 135) {
		PlayHitReactMontage(FName("FromRight"));
	}
	else if (Theta<-45 && Theta>-135) {
		PlayHitReactMontage(FName("FromLeft"));
	}
	else if (Theta > 135 || Theta < -135) {
		PlayHitReactMontage(FName("FromBack"));
	}
	else {
		PlayHitReactMontage(FName("FromFront"));
	}
}

bool ABaseCharacter::InTargetRange(AActor* Target, double Range)
{
	if (Target == nullptr) return false;
	const double Distance = (Target->GetActorLocation() - GetActorLocation()).Size();
	return Distance <= Range;
}

void ABaseCharacter::RegenPoise()
{
	if (Attribute) {
		Attribute->IncreasePoise();
		float PoisePercent = Attribute->CalculatePoise();
		if (PoisePercent != 0)
			GetWorldTimerManager().SetTimer(PoiseRegen, this, &ABaseCharacter::RegenPoise, LaterRegenTime);
	}
}

void ABaseCharacter::PawnSeen(APawn* SeenPawn)
{
}

void ABaseCharacter::Die_Implementation()
{
	if (DeathMontageSections.Num() == 0) return;
	int32 Section = FMath::RandRange(0, DeathMontageSections.Num() - 1);
	SetWeaponCollision(ECollisionEnabled::NoCollision);
	Tags.Add(FName("Dead"));
	PlayDeathReactMontage(FName(DeathMontageSections[Section]));
		
	TEnumAsByte<EDeathPose> Pose (Section);
	if (Pose < EDeathPose::EDP_MAX)
		DeathPose = Pose;

	GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void ABaseCharacter::Attack()
{
}

FVector ABaseCharacter::GetTranslationWarpTarget()
{
	if (CombatTarget==nullptr)	
	return FVector();
	const FVector TargetLocation = CombatTarget->GetActorLocation();
	const FVector Location = GetActorLocation();
	FVector TargetToActor = (Location - TargetLocation).GetSafeNormal();
	TargetToActor *= WarpTargetDistance;
	return TargetLocation + TargetToActor;
}

FVector ABaseCharacter::GetRotationWarpTarget()
{
	if (CombatTarget) {
		return CombatTarget->GetActorLocation();
	}
	return FVector();
}

void ABaseCharacter::PlayHitReactMontage(const FName& Name)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && ReactToHitMontage) {
		AnimInstance->Montage_Play(ReactToHitMontage, 1.35f);
		AnimInstance->Montage_JumpToSection(Name);
	}
}

void ABaseCharacter::PlayDeathReactMontage(const FName& Name)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage) {
		AnimInstance->Montage_Stop(0.f);
		AnimInstance->Montage_Play(DeathMontage, 1.f);
		AnimInstance->Montage_JumpToSection(Name);
	}
}


