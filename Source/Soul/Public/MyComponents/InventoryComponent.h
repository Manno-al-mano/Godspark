// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyComponents/HUDControlComponent.h"
#include "InventoryComponent.generated.h"

class AMyWeapon;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOUL_API UInventoryComponent : public UHUDControlComponent
{
	GENERATED_BODY()
};
