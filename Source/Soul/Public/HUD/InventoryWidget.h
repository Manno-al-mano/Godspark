// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void OpenMenu(class AMyCharacter* Character, TArray<class AMyWeapon*>Inventory);
	void CloseMenu();
	bool IsMenuOpen();
	void SetFocusedButton(int ButtonIndex);
	void MoveAroundMenu(float Direction);

	class UInventoryButton* AddOptionButton(const FText& WeaponName, bool bFocused = false);
	void PressButton();
	void SetDescription(AMyWeapon* Weapon);
private:
	void SelectNextButton();
	void SelectPreviousButton();
	void SetEquippedWeaponColor();
	bool bIsMenuOpen = false;
	
	class UInventoryButton* EquippedWeaponButton;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ItemBox;
	UPROPERTY(meta = (BindWidget))
	class UInventoryButton* SampleButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SampleButtonText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NameText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DamageText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SpeedText;
	UPROPERTY(meta = (BindWidget))
	class UMultiLineEditableText* DescriptionText;
	TArray<UInventoryButton*> InventoryButtons;
	UInventoryButton* FocusedButton;
	int32 SelectedIndex;
	
};
