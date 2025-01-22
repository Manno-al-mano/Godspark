// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHUD.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

void UPlayerHUD::SetHealthPercent(float Percent)
{
	
	if (!HealthBar) return;
	DesiredHealth = Percent;
	FillHP();
}

void UPlayerHUD::SetPoisePercent(float Percent)
{
	
	if (!PoiseBar) return;
	DesiredPoise = Percent;
	FillPoise();
}

void UPlayerHUD::SetStaminaPercent(float Percent)
{
	if (!StaminaBar) return;
	DesiredStamina = Percent;
	FillStamina();
}

void UPlayerHUD::SetStaminaPercentFast(float Percent)
{
	if (!StaminaBar) return;
	StaminaBar->SetPercent(Percent);
}

void UPlayerHUD::SetFocusVisibility(bool bVisibility)
{
	if (FocusWidget)FocusWidget->SetVisibility(bVisibility?ESlateVisibility::Visible:ESlateVisibility::Collapsed);
}

void UPlayerHUD::SetFocusLocation(FVector2D ScreenPosition)
{
	if (!FocusWidget) return;
	if (!FocusSlot) {
		 FocusSlot = Cast<UCanvasPanelSlot>(FocusWidget->Slot);
	}
	if(FocusSlot)
	{
			FocusSlot->SetPosition(ScreenPosition);
	}


}

void UPlayerHUD::SetGoldCount(int32 Count)
{
	if (GoldCount)
		GoldCount->SetText(FText::FromString(FString::Printf(TEXT("%d"),Count)));
}

void UPlayerHUD::SetActionText(const FText& text)
{
	if (ActionText) ActionText->SetText(text);
	ActionText->SetVisibility(ESlateVisibility::Visible);
}
void UPlayerHUD::DisableActionText()
{
	if(ActionText)
		ActionText->SetVisibility(ESlateVisibility::Hidden);
}
void UPlayerHUD::SetSoulCount(int32 Count)
{
	if (SoulCount)
		SoulCount->SetText(FText::FromString(FString::Printf(TEXT("%d"), Count)));
}

void UPlayerHUD::FillHP()
{
	UWorld* World = GetWorld();
	if (!World) return;
	APlayerController* OwningPlayerController = World->GetFirstPlayerController();
	if(OwningPlayerController)
	{
		float PercentCurrent = HealthBar->GetPercent();
		HealthBar->SetPercent(PercentCurrent + (DesiredHealth - PercentCurrent) * 0.1f);
		if (!FMath::IsNearlyEqual(DesiredHealth, PercentCurrent, 0.01f))
		{
			OwningPlayerController->GetWorldTimerManager().SetTimer(HPFillUp, this, &UPlayerHUD::FillHP, 0.01f);
		}
		else {
			HealthBar->SetPercent(DesiredHealth);
		}
	}
}

void UPlayerHUD::FillPoise()
{
	UWorld* World = GetWorld();
	if (!World) return;
	APlayerController* OwningPlayerController = World->GetFirstPlayerController();
	if (OwningPlayerController)
	{
		float PercentCurrent = PoiseBar->GetPercent();
		float Diff = DesiredPoise - PercentCurrent;
		if (Diff > 0.01f || Diff < -0.01f)
			PoiseBar->SetPercent(PercentCurrent + (Diff) * 0.1f);
		else
			PoiseBar->SetPercent(DesiredPoise);
		if (PoiseHighlight) {
			PoiseHighlight->SetOpacity(PercentCurrent == 1.f);
		}
		if (DesiredPoise != PercentCurrent)
			OwningPlayerController->GetWorldTimerManager().SetTimer(PoiseFillUp, this, &UPlayerHUD::FillPoise, 0.01f);
	}
}

void UPlayerHUD::FillStamina()
{
	UWorld* World = GetWorld();
	if (!World) return;
	APlayerController* OwningPlayerController = World->GetFirstPlayerController();
	if (OwningPlayerController)
	{
		float PercentCurrent = StaminaBar->GetPercent();
		StaminaBar->SetPercent(PercentCurrent + (DesiredStamina - PercentCurrent) * 0.1f);
		if (!FMath::IsNearlyEqual(DesiredStamina, PercentCurrent, 0.01f))
		{
			OwningPlayerController->GetWorldTimerManager().SetTimer(StaminaFillUp, this, &UPlayerHUD::FillStamina, 0.01f);
		}
		else {
			StaminaBar->SetPercent(DesiredStamina);
		}
	}
}
