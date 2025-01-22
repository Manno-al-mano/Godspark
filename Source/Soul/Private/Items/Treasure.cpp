// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Treasure.h"
#include "characters/MyCharacter.h"



ATreasure::ATreasure()
{
	ItemState = EItemState::EIS_Lie;
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IInteractInterface* Character = Cast<IInteractInterface>(OtherActor);
	if (Character) {
		PlayPickUpSound();
		Character->AddGold(this);
		Destroy();
	}
}


