// Fill out your copyright notice in the Description page of Project Settings.


#include "MyComponents/AttributeComponent.h"

UAttributeComponent::UAttributeComponent()
{
	
	PrimaryComponentTick.bCanEverTick = false;

}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	
}
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UAttributeComponent::DamagePoise()
{
	if (Poise < 0) return;
	HitCount++;
	if (HitCount > Poise) { 
		HitCount = 0;
	}

}

bool UAttributeComponent::DecreaseStamina(float Amount)
{
	Stamina = FMath::Clamp(Stamina - Amount, 0.f,MaxStamina);
	return Stamina == 0.f;
}


