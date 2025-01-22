// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/InteractInterface.h"

// Add default functionality here for any IInteractInterface functions that are not pure virtual.

void IInteractInterface::SetOverlappingItem(IInteractableInterface* Item)
{
}

void IInteractInterface::SetAction(const FText& Action)
{
}


void IInteractInterface::AddSouls(ASoulItem* Soul)
{
}

void IInteractInterface::AddGold(ATreasure* Gold)
{
}
