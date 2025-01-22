// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	void SetHealthPercent(float Percent);
	void SetPoisePercent(float Percent);
	void SetName(const FText& InputName);
	void SetTextColor(FLinearColor Color);
	void AddToHUD();
	void RemoveFromHUD();
	

protected:
	UPROPERTY()
	class UHealthBar* HealthBarWidget;
	float DesiredHealth;
	float DesiredPoise;
	void FillHP();
	void FillPoise();
	FTimerHandle HPFillUp;
	FTimerHandle PoiseFillUp;

	
};
