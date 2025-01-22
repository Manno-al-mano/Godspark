// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SoulHUD.generated.h"
class UPlayerHUD;
class UDialogueWidget;
class UInventoryWidget;
/**
 * 
 */
UCLASS()
class SOUL_API ASoulHUD : public AHUD
{
	GENERATED_BODY()
public:
	void AddBossBar(class UHealthBar* Widget);
	void RemoveBossBar(UHealthBar* Widget);
protected:
	virtual void BeginPlay() override;
	
private:
	UPlayerHUD* PlayerHUD;
	UDialogueWidget* DialogueWidget;
	UInventoryWidget* InventoryWidget;
	UHealthBar* BossBar;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerHUD> PlayerHUDClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDialogueWidget> DialogueWidgetClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;
public:
	FORCEINLINE UPlayerHUD* GetPlayerHUD() const { return PlayerHUD; }
	FORCEINLINE UDialogueWidget* GetDialogueWidget() const { return DialogueWidget; }
	FORCEINLINE UInventoryWidget* GetInventoryWidget() const { return InventoryWidget; }
};
