// Fill out your copyright notice in the Description page of Project Settings.


#include "characters/enemies/Boss.h"
#include "HUD/HealthBarComponent.h"


void ABoss::PawnSeen(APawn* SeenPawn)
{
	
	const bool bShouldChase = EnemyState < EEnemyState::EES_Chasing
		&& !IsDead()
		&& SeenPawn->ActorHasTag(FName("PlayerFaction"))
		&& !SeenPawn->ActorHasTag(FName("Dead"));
	if (bShouldChase) {
		Super::PawnSeen(SeenPawn);
		if (HealthBarComponent) HealthBarComponent->AddToHUD();
	}
	
}

void ABoss::ChangeHealthbarVisibility(bool status)
{
	if (HealthBarComponent) HealthBarComponent->SetVisibility(false);
}

void ABoss::Die_Implementation()
{
	if (HealthBarComponent)HealthBarComponent->RemoveFromHUD();
	Super::Die_Implementation();
}
