// Fill out your copyright notice in the Description page of Project Settings.


#include "MyComponents/DialogueComponent.h"
#include "HUD/DialogueWidget.h"
#include "HUD/SoulHUD.h"
#include "HUD/DialogueButton.h"
#include "characters/MyCharacter.h"
//#include "Macros.h"

// Sets default values for this component's properties
UDialogueComponent::UDialogueComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}



void UDialogueComponent::StartMenu(AMyCharacter* Player)
{
	if (!DialogueWidget) {
		if(!TryGetWidget()) return;
	}
	if (!SetPlayer(Player))
	{
		return;
	}
	DialogueWidget->SetNameText(Name);
	DialogueWidget->SetVisibility(ESlateVisibility::Visible);
	PlayDialogueBranch(GreetingIndex);
	GreetingIndex = SecondTimeGreetingIndex;
	bDialogueStarted = true;
	DialogueWidget->SetDialogueComponent(this);
}

void UDialogueComponent::EndMenu()
{
	if (!DialogueWidget) return;
	DialogueWidget->SetVisibility(ESlateVisibility::Hidden);
	bDialogueStarted = false;
	SetPlayer(nullptr);
	DialogueWidget->SetDialogueComponent(nullptr);
}

void UDialogueComponent::PlayerAction()
{

	if (!DialogueWidget) return;
	if (!DialogueWidget->GetResponseVisibility())
	{
		GetWorld()->GetTimerManager().ClearTimer(DialogueTimer);
		PlayNextDialogueLine();
	}
	else
		DialogueWidget->PressButton();
}

void UDialogueComponent::TriggerDialogueResponse(int Index)
{
	
	if (Index < 0 || Index >= DialogueResponses.Num()) return;
	if (DialogueResponses[Index].bCanBeAskedOnce) DialogueResponses[Index].bIsAvailable = false;
	for (int32 Ind : DialogueResponses[Index].ResponsesEnabled)
		if (Ind > 0 && Ind < DialogueResponses.Num()) DialogueResponses[Ind].bIsAvailable = true;
	PlayDialogueBranch(DialogueResponses[Index].ResponseIndex);
}

void UDialogueComponent::MoveAroundMenu(float Direction)
{
	if (!DialogueWidget) return;
	if (Direction < 0)
		DialogueWidget->SelectNextButton();
	if (Direction > 0)
		DialogueWidget->SelectPreviousButton();

}
bool UDialogueComponent::SetPlayer(AMyCharacter* Player)
{
	if (Player) {
		PlayerCharacter = Player;
		PlayerCharacter->SetDialogueComponent(this);
		PlayerCharacter->DisablePlayerHUD();
		return true;
	}
	else {
		if (PlayerCharacter)
		{
			PlayerCharacter->EnablePlayerHUD();
			PlayerCharacter->SetDialogueComponent(nullptr);
		}
		PlayerCharacter = nullptr;
		return false;
	}
}



void UDialogueComponent::PlayNextDialogueLine()
{
	if (!DialogueWidget) return;
	if (DialogueLineIndex >= DialogueTree[CurrentDialogue].DialogueLines.Num())
	{
		ShowPlayerOptions();
		return;
	}
	DialogueWidget->SetDialogueText(DialogueTree[CurrentDialogue].DialogueLines[DialogueLineIndex]);
	DialogueLineIndex++;
	GetWorld()->GetTimerManager().SetTimer(DialogueTimer,this, &UDialogueComponent::PlayNextDialogueLine, DialogueTime);
}

void UDialogueComponent::PlayDialogueBranch(int32 index)
{
	if (!DialogueWidget) return;
	if (index < 0 || index >= DialogueTree.Num()) return;
	DialogueWidget->SetResponseVisibility(false);
	CurrentDialogue = index;
	DialogueLineIndex = 0;
	PlayNextDialogueLine();
}


void UDialogueComponent::ShowPlayerOptions()
{
	if (!DialogueWidget) return;
	DialogueWidget->SetResponseVisibility(true);
	DialogueWidget->SetDialogueText(FText());
	DialogueWidget->ClearOptions();
	for (int i = 0; i < DialogueResponses.Num();i++) {
		if (DialogueResponses[i].bIsAvailable) {
			UDialogueButton* ResponseButton = DialogueWidget->AddOptionButton(DialogueResponses[i].ResponseText);
			if (ResponseButton) ResponseButton->SetupClickEvent(i,this);
		}
	}
	UDialogueButton* ResponseButton = DialogueWidget->AddOptionButton(Exit,true);
	if (ResponseButton) ResponseButton->SetupEndDialogueEvent(this);
}

bool UDialogueComponent::TryGetWidget()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController) {
		ASoulHUD* SoulHUD = Cast<ASoulHUD>(PlayerController->GetHUD());
		if (SoulHUD) {
			DialogueWidget = SoulHUD->GetDialogueWidget();
			if (DialogueWidget)
				return true;
		}
	}
	return false;
}





// Called every frame
void UDialogueComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

