// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HUDControlComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOUL_API UHUDControlComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHUDControlComponent();
	virtual void PlayerAction();
	virtual void MoveAroundMenu(float Direction);
	virtual void StartMenu(class AMyCharacter* Player);
	
	virtual void EndMenu();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual bool TryGetWidget();
	class AMyCharacter* PlayerCharacter;
	bool SetPlayer(AMyCharacter* Player);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
