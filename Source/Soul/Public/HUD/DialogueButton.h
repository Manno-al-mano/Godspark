// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "MyComponents/DialogueComponent.h"
#include "DialogueButton.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_API UDialogueButton : public UButton
{

	GENERATED_BODY()
public:
	void SetupClickEvent(int32 index, UDialogueComponent* Dialogue);
	void SetupEndDialogueEvent(UDialogueComponent* Dialogue);
	
protected:
	int32 ResponseIndex;
	UDialogueComponent* DialogueComponent;
	UFUNCTION()
	void TriggerDialogue();
	UFUNCTION()
	void TriggerEndDialogue();
public:
};
