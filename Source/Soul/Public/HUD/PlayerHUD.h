// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/HealthBar.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_API UPlayerHUD : public UUserWidget
{

	GENERATED_BODY()
public:
	void SetHealthPercent(float Percent);
	void SetPoisePercent(float Percent);
	void SetStaminaPercent(float Percent);
	void SetStaminaPercentFast(float Percent);
	void SetFocusVisibility(bool bVisibility);
	void SetFocusLocation(FVector2D ScreenPosition);
	void SetGoldCount(int32 Count);
	void SetActionText(const FText& text);
	void DisableActionText();
	void SetSoulCount(int32 Count);
	//protected void BeginPlay() override;
private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SoulCount;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GoldCount;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ActionText;
	UPROPERTY(meta = (BindWidget))
	class UImage* FocusWidget;
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* StaminaBar;
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* PoiseBar;
	UPROPERTY(meta = (BindWidget))
	class UImage* PoiseHighlight;
	class UCanvasPanelSlot* FocusSlot;
	void FillHP();
	void FillPoise();
	void FillStamina();
	float DesiredHealth;
	float DesiredStamina;
	float DesiredPoise;
	FTimerHandle HPFillUp;
	FTimerHandle PoiseFillUp;
	FTimerHandle StaminaFillUp;

	
};
