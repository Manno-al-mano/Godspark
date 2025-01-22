// Fill out your copyright notice in the Description page of Project Settings.


#include "MyComponents/HUDControlComponent.h"
#include "characters/MyCharacter.h"

// Sets default values for this component's properties
UHUDControlComponent::UHUDControlComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UHUDControlComponent::PlayerAction()
{
}

void UHUDControlComponent::MoveAroundMenu(float Direction)
{
}

void UHUDControlComponent::StartMenu(AMyCharacter* Player)
{
}

void UHUDControlComponent::EndMenu()
{
}


// Called when the game starts
void UHUDControlComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

bool UHUDControlComponent::TryGetWidget()
{
	return false;
}



// Called every frame
void UHUDControlComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

