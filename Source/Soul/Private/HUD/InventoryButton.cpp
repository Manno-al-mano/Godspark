// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/InventoryButton.h"
#include "characters/MyCharacter.h"

void UInventoryButton::SetupClickEvent(int32 Index, AMyCharacter* Character, AMyWeapon* Weapon)
{
	WeaponIndex = Index;
	MyCharacter = Character;
	MyWeapon = Weapon;
	OnClicked.AddDynamic(this, &UInventoryButton::Click);
}
void UInventoryButton::Click() {
	if(MyCharacter) MyCharacter->EquipFromInventory(WeaponIndex);
}
