// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DialogueWidget.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	
	void SetNameText(FText Text);
	void SetDialogueText(FText Text);
	void ClearOptions();
	void SetFocusedButton(int ButtonIndex);
	void SelectNextButton();
	void SelectPreviousButton();
	class UDialogueButton* AddOptionButton(const FText& Response, bool bFocused = false);
	void PressButton();
	void SetResponseVisibility(bool bEnabled);
	bool GetResponseVisibility();

private:
	UPROPERTY(meta = (BindWidget))
	class UImage* ResponseBoxImage;
	class UDialogueComponent* DialogueComponent;
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ResponseOptionBox;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NameText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DialogueText;
	UPROPERTY(meta = (BindWidget))
	class UDialogueButton* SampleButton;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SampleButtonText;
	TArray<UDialogueButton*> DialogueButtons;
	UDialogueButton* FocusedButton;
	int32 SelectedIndex;
public:
	FORCEINLINE void SetDialogueComponent(UDialogueComponent* Dialogue) { DialogueComponent = Dialogue; };
};
