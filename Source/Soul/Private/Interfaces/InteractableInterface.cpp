// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/InteractableInterface.h"


// Add default functionality here for any IInteractableInterface functions that are not pure virtual.

void IInteractableInterface::Interact(AMyCharacter* Character)
{
}

bool IInteractableInterface::DoesHaveTag(const FName& Tag)
{
	return false;
}

AMyWeapon* IInteractableInterface::GetIfWeapon()
{
	return nullptr;
}

EInteractableType IInteractableInterface::CheckType()
{
	return EInteractableType::EIT_None;
}
