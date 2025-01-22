// Fill out your copyright notice in the Description page of Project Settings.


#include "characters/enemies/Ally.h"
#include "characters/MyCharacter.h"
#include "HUD/HealthBarComponent.h"
#include "Items/MyWeapon.h"
#include "MyComponents/MercenaryDialogueComponent.h"
#include "Interfaces/InteractInterface.h"
#include "components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
//#include "Macros.h"

AAlly::AAlly():Super()
{
	Sphere = CreateDefaultSubobject<USphereComponent>(FName("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());
	DialogueComponent = CreateDefaultSubobject<UMercenaryDialogueComponent>(FName("Dialogue Component"));
}

void AAlly::Interact(AMyCharacter* Character)
{
	
	DialogueComponent->StartMenu(Character);
	//	AActor* Actor = Cast<AActor>(Character);
	//if(Actor&&!bShouldStandUp)
	//{
	//	Character->SetAction(ActionText);
	//	PatrolTargetCurrent = Actor;
	//	PatrolTargets.AddUnique(Actor);
	//	bHasSeenPlayer = true;
	//	Sphere->Deactivate();
	//	Sphere->DestroyComponent();
	//	ChangeHealthbarVisibility(true);
	//	HealthBarComponent->SetTextColor(FColor::Green);
	//	bShouldStandUp = true;
	//
	//}
}

bool AAlly::DoesHaveTag(const FName& Tag)
{
	return ActorHasTag(Tag);
} 

void AAlly::StartService(AMyCharacter* Character)
{
	
	PatrolTargetCurrent = Character;
	PatrolTargets.AddUnique(Character);
	bHasSeenPlayer = true;
	Sphere->Deactivate();
	Sphere->DestroyComponent();
	ChangeHealthbarVisibility(true);
	bShouldStandUp = true;
}

void AAlly::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IInteractInterface* Character = Cast<IInteractInterface>(OtherActor);
	if (Character) {
		Character->SetOverlappingItem(this);
		Character->SetAction(ActionText);
	}
}

void AAlly::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IInteractInterface* Character = Cast<IInteractInterface>(OtherActor);
	if (Character) {
		Character->SetOverlappingItem(nullptr);
	}
}

void AAlly::BeginPlay()
{
	Super::BeginPlay();
	if(GetMovementComponent())
	GetMovementComponent()->Deactivate();
	Tags.Remove(FName("EnemyFaction"));
	Tags.Add(FName("PlayerFaction"));
	Tags.Add(FName("DialogueClass"));
	EnemyState = EEnemyState::EES_Staggered;
	EquippedWeapon->SetVisibility(false);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAlly::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AAlly::OnSphereOverlapEnd);
	DialogueComponent->SetAlly(this);
}


void AAlly::PawnSeen(APawn* SeenPawn)
{
	
		const bool bShouldChase = EnemyState < EEnemyState::EES_Chasing
			&& !IsDead()
			&& SeenPawn->ActorHasTag(FName("EnemyFaction"))
			&& !SeenPawn->ActorHasTag(FName("Dead"));
		if (bShouldChase) {
			CombatTarget = SeenPawn;
			bShouldStandUp = true;
			StartChasing();

		}
	
}

void AAlly::LoseTarget()
{
	CombatTarget = nullptr;
	EnemyState = EEnemyState::EES_Patrolling;
}

void AAlly::StandUp()
{
	bHasStoodUp = true;
	EquippedWeapon->SetVisibility(true);
	EnemyState = EEnemyState::EES_Patrolling;
	StartMovement(PatrolTargetCurrent);
	GetMovementComponent()-> Activate(true);
}
