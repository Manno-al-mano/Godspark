// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyComponents/DialogueComponent.h"
#include "MercenaryDialogueComponent.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_API UMercenaryDialogueComponent : public UDialogueComponent
{
	GENERATED_BODY()
public: 
	void TriggerDialogueResponse(int Index) override;
	void SetAlly(class AAlly* Ally);
protected:
	class AAlly* MyAlly;
	virtual void PlayNextDialogueLine() override;
	UPROPERTY(EditAnywhere)
	int32 HiringResponseIndex=0;
	UPROPERTY(EditAnywhere)
	int32 Price=100;
	UPROPERTY(EditAnywhere)
	int32 EnoughMoneyResponse = 0;

	
};
