// Fill out your copyright notice in the Description page of Project Settings.


#include "MyComponents/MercenaryDialogueComponent.h"
#include "characters/MyCharacter.h"
#include "characters/enemies/Ally.h"

void UMercenaryDialogueComponent::TriggerDialogueResponse(int Index)
{
	if (Index < 0 || Index >= DialogueResponses.Num() || !PlayerCharacter) return;
	if (DialogueResponses[Index].bCanBeAskedOnce) DialogueResponses[Index].bIsAvailable = false;
	for (int32 Ind : DialogueResponses[Index].ResponsesEnabled)
		if (Ind > 0 && Ind < DialogueResponses.Num()) DialogueResponses[Ind].bIsAvailable = true;
	if (Index!= HiringResponseIndex ||PlayerCharacter->GetGold() < Price)
	PlayDialogueBranch(DialogueResponses[Index].ResponseIndex);
	else {
		PlayDialogueBranch(EnoughMoneyResponse);
		PlayerCharacter->AddGold(-Price);
	}

}

void UMercenaryDialogueComponent::SetAlly(AAlly* Ally)
{
	MyAlly = Ally;
}

void UMercenaryDialogueComponent::PlayNextDialogueLine()
{
	if (!DialogueWidget && !PlayerCharacter) return;
	if (DialogueLineIndex >= DialogueTree[CurrentDialogue].DialogueLines.Num()&& CurrentDialogue == EnoughMoneyResponse)
	{
		if (MyAlly) 
			MyAlly->StartService(PlayerCharacter);
			EndMenu();
			return;
			
	}
	else Super::PlayNextDialogueLine();
}
