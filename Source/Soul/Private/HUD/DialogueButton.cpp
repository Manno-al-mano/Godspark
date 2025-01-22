// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/DialogueButton.h"
#include "Macros.h"

void UDialogueButton::SetupClickEvent(int32 index, UDialogueComponent* Dialogue)
{
	ResponseIndex = index;
	DialogueComponent = Dialogue;
	OnClicked.AddDynamic(this, &UDialogueButton::TriggerDialogue);
}

void UDialogueButton::SetupEndDialogueEvent(UDialogueComponent* Dialogue)
{
	DialogueComponent = Dialogue;
	OnClicked.AddDynamic(this, &UDialogueButton::TriggerEndDialogue);
}
void UDialogueButton::TriggerEndDialogue() {
	if(DialogueComponent)
	DialogueComponent->EndMenu();

}

void UDialogueButton::TriggerDialogue()
{
	if (DialogueComponent)
	DialogueComponent->TriggerDialogueResponse(ResponseIndex);
}
