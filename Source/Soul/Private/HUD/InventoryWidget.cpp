// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/InventoryWidget.h"
#include "HUD/InventoryButton.h"
#include "MyComponents/DialogueComponent.h"
#include "Components/TextBlock.h"
#include "Components/MultiLineEditableText.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Blueprint/WidgetTree.h"
#include "Styling/SlateTypes.h"
#include "characters/MyCharacter.h"
#include "Items/MyWeapon.h"

void UInventoryWidget::OpenMenu(AMyCharacter* Character, TArray<AMyWeapon*>Inventory)
{
	if (ItemBox) ItemBox->ClearChildren();
	InventoryButtons.Empty();
	SetVisibility(ESlateVisibility::Visible);
	bIsMenuOpen = true;
	for (int32 i = 0; i < Inventory.Num(); i++) {
		UInventoryButton* Button;
		if (i == 0) {
		Button = AddOptionButton(Inventory[i]->GetItemName(), true);
		}
		else Button = AddOptionButton(Inventory[i]->GetItemName());
		
		Button->SetupClickEvent(i, Character, Inventory[i]);
			SetDescription(FocusedButton->GetMyWeapon());
			if (Button->GetMyWeapon() == Character->GetEquippedWeapon()) {
				EquippedWeaponButton = Button;
				if(Button != FocusedButton)SetEquippedWeaponColor();
		}
	}
}

void UInventoryWidget::CloseMenu()
{
	SetVisibility(ESlateVisibility::Hidden);
	bIsMenuOpen = false;
}

bool UInventoryWidget::IsMenuOpen()
{
	return bIsMenuOpen;
}

void  UInventoryWidget::SetFocusedButton(int ButtonIndex) {
	if (FocusedButton) {
		if (FocusedButton == EquippedWeaponButton)
			SetEquippedWeaponColor();
		else
		FocusedButton->SetStyle(SampleButton->GetStyle());
	}

	if (ButtonIndex >= InventoryButtons.Num()) return;
	FocusedButton = InventoryButtons[ButtonIndex];
	FButtonStyle ButtonStyle = FocusedButton->GetStyle();
	ButtonStyle.Normal.TintColor = FLinearColor(0.75f, .75f, .75f, 0.75f);
	FocusedButton->SetStyle(ButtonStyle);
}

void UInventoryWidget::MoveAroundMenu(float Direction)
{
	if (Direction < 0)
		SelectNextButton();
	if (Direction > 0)
		SelectPreviousButton();

}


void UInventoryWidget::SelectNextButton() {
	if (SelectedIndex >= InventoryButtons.Num() - 1) return;
	SelectedIndex++;
	SetFocusedButton(SelectedIndex);
	SetDescription(FocusedButton->GetMyWeapon());
}

void UInventoryWidget::SelectPreviousButton() {
	if (SelectedIndex <= 0) return;
	SelectedIndex--;
	SetFocusedButton(SelectedIndex);
	SetDescription(FocusedButton->GetMyWeapon());
}

void UInventoryWidget::SetEquippedWeaponColor()
{
	FButtonStyle  Style = EquippedWeaponButton->GetStyle();
	Style.Normal.TintColor = FLinearColor(0.5f, 0.f, 1.f, 0.75f);
	EquippedWeaponButton->SetStyle(Style);
}

UInventoryButton* UInventoryWidget::AddOptionButton(const FText& WeaponName, bool bFocused)
{
	if (!ItemBox || !SampleButton || !SampleButtonText) return nullptr;

	UInventoryButton* InventoryButton = DuplicateObject<UInventoryButton>(SampleButton, nullptr);
	if (!InventoryButton) return nullptr;

	UTextBlock* TextBlock = DuplicateObject<UTextBlock>(SampleButtonText, nullptr);
	if (!TextBlock) return nullptr;

	TextBlock->SetText(WeaponName);
	InventoryButton->AddChild(TextBlock);
	ItemBox->AddChild(InventoryButton);
	InventoryButtons.Add(InventoryButton);
	if (bFocused)
	{
		SelectedIndex = InventoryButtons.Num() - 1;
		SetFocusedButton(SelectedIndex);

	}

	return InventoryButton;

}

void UInventoryWidget::PressButton() {
	if (FocusedButton) {
		FocusedButton->OnClicked.Broadcast();
		if (EquippedWeaponButton != FocusedButton) EquippedWeaponButton->SetStyle(SampleButton->GetStyle());
		EquippedWeaponButton=FocusedButton;
	}
}

void UInventoryWidget::SetDescription(AMyWeapon* Weapon)
{
	if (!Weapon) return;
	if(NameText)
	NameText->SetText(Weapon->GetItemName());
	if (DamageText) {
		FText DamageLabelText = FText::FromString("Damage: ");
		FText DamageValueText = FText::AsNumber(Weapon->GetDamage());
		FText DamageDescription = FText::Format(FText::FromString("{0}{1}"), DamageLabelText, DamageValueText);
		DamageText->SetText(DamageDescription);
	}
	if (SpeedText)
	{
		FText SpeedLabelText = FText::FromString("Attack Speed: ");
		FText SpeedValueText = FText::AsNumber(Weapon->GetAttackSpeed() * 100);
		FText SpeedLabelEnd = FText::FromString("%");
		FText SpeedDescription = FText::Format(FText::FromString("{0}{1}{2}"), SpeedLabelText, SpeedValueText,SpeedLabelEnd);
		SpeedText->SetText(SpeedDescription);
	}
	if(DescriptionText)
		DescriptionText->SetText(Weapon->GetDescription());
}


