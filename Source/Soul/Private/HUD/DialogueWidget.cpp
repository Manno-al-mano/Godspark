// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/DialogueWidget.h"
#include "HUD/DialogueButton.h"
#include "MyComponents/DialogueComponent.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Blueprint/WidgetTree.h"
#include "Styling/SlateTypes.h"

void UDialogueWidget::SetNameText(FText Text)
{
	if (NameText)
		NameText->SetText(Text);
}

void UDialogueWidget::SetDialogueText(FText Text)
{
	if (DialogueText)
		DialogueText->SetText(Text);
}

void UDialogueWidget::ClearOptions()
{
	if (ResponseOptionBox) ResponseOptionBox->ClearChildren();
	DialogueButtons.Empty();
}
void  UDialogueWidget::SetFocusedButton(int ButtonIndex) {
	if (FocusedButton) {
		FocusedButton->SetStyle(SampleButton->GetStyle());
	}
	ResponseOptionBox->ScrollWidgetIntoView(ResponseOptionBox->GetChildAt(ButtonIndex));

	if (ButtonIndex >= DialogueButtons.Num()) return;
	FocusedButton = DialogueButtons[ButtonIndex];
	//FocusedButton->SetKeyboardFocus();
	FButtonStyle ButtonStyle = FocusedButton->GetStyle();
	ButtonStyle.Normal.TintColor= FLinearColor(0.75f, .75f, .75f, 0.75f);
	FocusedButton->SetStyle(ButtonStyle);
}

void UDialogueWidget::SelectNextButton() {
	if (SelectedIndex >= DialogueButtons.Num()-1) return;
	SelectedIndex++;
	SetFocusedButton(SelectedIndex);
}

void UDialogueWidget::SelectPreviousButton() {
	if (SelectedIndex <= 0) return;
	SelectedIndex--;
	SetFocusedButton(SelectedIndex);
		
}

UDialogueButton* UDialogueWidget::AddOptionButton(const FText& Response, bool bFocused)
{
if (!ResponseOptionBox || !SampleButton || !SampleButtonText) return nullptr;

UDialogueButton* ResponseButton = DuplicateObject<UDialogueButton>(SampleButton, nullptr);
if (!ResponseButton) return nullptr;

UTextBlock* TextBlock = DuplicateObject<UTextBlock>(SampleButtonText, nullptr);
if (!TextBlock) return nullptr;

TextBlock->SetText(Response);
ResponseButton->AddChild(TextBlock);
ResponseOptionBox->AddChild(ResponseButton);
DialogueButtons.Add(ResponseButton);
if (bFocused)
{
	SelectedIndex = DialogueButtons.Num()-1;
	SetFocusedButton(SelectedIndex);
}
return ResponseButton;

}
void UDialogueWidget::PressButton(){ 
	if (FocusedButton)FocusedButton->OnClicked.Broadcast(); 
}

void UDialogueWidget::SetResponseVisibility(bool bEnabled)
{
    if (ResponseOptionBox)
        ResponseOptionBox->SetVisibility(bEnabled ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	if(ResponseBoxImage)
		ResponseBoxImage->SetVisibility(bEnabled ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

bool UDialogueWidget::GetResponseVisibility()
{
	if (ResponseOptionBox && ResponseOptionBox->GetVisibility() == ESlateVisibility::Visible)
		return true;
	return false;
}


