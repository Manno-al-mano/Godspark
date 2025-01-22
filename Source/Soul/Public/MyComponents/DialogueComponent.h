// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DialogueComponent.generated.h"


USTRUCT(BlueprintType)
struct FDialogueResponse
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FText ResponseText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAvailable = false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 ResponseIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> ResponsesEnabled;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanBeAskedOnce = false;

};

USTRUCT(BlueprintType)
struct FDialogueBranch
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FText> DialogueLines;
};

DECLARE_DELEGATE(FOnDialogueEnd);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOUL_API UDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDialogueComponent();
	void StartMenu(class AMyCharacter* Player);
	UFUNCTION()
	void EndMenu();
	void PlayerAction();
	void MoveAroundMenu(float Direction);
	virtual void TriggerDialogueResponse(int Index);
	


protected:
	class AMyCharacter* PlayerCharacter;
	bool SetPlayer(AMyCharacter* Player);
	bool TryGetWidget();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(EditAnywhere)
	FText Name;
	UPROPERTY(EditAnywhere)
	FText Exit;
	UPROPERTY(EditAnywhere)
	TArray<FDialogueBranch> DialogueTree;
	UPROPERTY(EditAnywhere)
	TArray<FDialogueResponse> DialogueResponses;
	class UDialogueWidget* DialogueWidget;
	UPROPERTY(EditAnywhere)
	float DialogueTime=10.f;
	FTimerHandle DialogueTimer;
	int32 DialogueLineIndex=0;
	int32 GreetingIndex=0;
	int32 SecondTimeGreetingIndex=1;
	int32 CurrentDialogue=0;
	virtual void PlayNextDialogueLine();
	UFUNCTION()
	virtual void PlayDialogueBranch(int32 index);
	bool bDialogueStarted=false;	
	void ShowPlayerOptions();
public:
	FORCEINLINE bool GetDialogueStarted() const { return bDialogueStarted; }
};
