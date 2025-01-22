// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "InventoryButton.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_API UInventoryButton : public UButton
{

	GENERATED_BODY()
public:
	void SetupClickEvent(int32 Index, class AMyCharacter* Character,class AMyWeapon* Weapon);
	UFUNCTION()
	void Click();
private:
	class AMyCharacter* MyCharacter;
	int32 WeaponIndex;
	class AMyWeapon* MyWeapon;
public:
	AMyWeapon* GetMyWeapon() { return MyWeapon; }

};
