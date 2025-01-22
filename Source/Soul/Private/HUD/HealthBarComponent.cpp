// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HealthBarComponent.h"
#include "HUD/HealthBar.h"
#include "HUD/SoulHUD.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


void UHealthBarComponent::SetHealthPercent(float Percent)
{
	if(HealthBarWidget==nullptr)
	HealthBarWidget = Cast<UHealthBar> (GetUserWidgetObject());
	if(HealthBarWidget)
	{
		DesiredHealth = Percent;
		FillHP();
	}
}






void UHealthBarComponent::FillHP()
{
	if (HealthBarWidget && HealthBarWidget->HealthBar)
	{
		float PercentCurrent = HealthBarWidget->HealthBar->GetPercent();
		HealthBarWidget->HealthBar->SetPercent(PercentCurrent+(DesiredHealth-PercentCurrent)*0.1f);
		if (!FMath::IsNearlyEqual(DesiredHealth, PercentCurrent, 0.01f))
		GetOwner()->GetWorldTimerManager().SetTimer( HPFillUp, this, &UHealthBarComponent::FillHP, 0.01f);
		else HealthBarWidget->HealthBar->SetPercent(DesiredHealth);
	}
}

void UHealthBarComponent::FillPoise()
{
	if (HealthBarWidget && HealthBarWidget->PoiseBar)
	{
		float PercentCurrent = HealthBarWidget->PoiseBar->GetPercent();
		float Diff = DesiredPoise - PercentCurrent;
		if(Diff>0.01f||Diff<-0.01f)
		HealthBarWidget->PoiseBar->SetPercent(PercentCurrent + (Diff) * 0.1f);
		else
			HealthBarWidget->PoiseBar->SetPercent(DesiredPoise);
		if (HealthBarWidget->PoiseHighlight) {
			HealthBarWidget->PoiseHighlight->SetOpacity(PercentCurrent == 1.f);
		}
		if (DesiredPoise != PercentCurrent)
		GetOwner()->GetWorldTimerManager().SetTimer(PoiseFillUp, this, &UHealthBarComponent::FillPoise, 0.01f);
	}
}


void UHealthBarComponent::SetPoisePercent(float Percent)
{
	if (HealthBarWidget == nullptr)
		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	if(HealthBarWidget)
	{
		DesiredPoise = Percent;
		FillPoise();
	}
}

void UHealthBarComponent::SetName(const FText& InputName)
{
	if (HealthBarWidget == nullptr)
		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	if (HealthBarWidget->InGameName)
		HealthBarWidget->InGameName->SetText(InputName);
}

void UHealthBarComponent::SetTextColor(FLinearColor Color)
{
	if (HealthBarWidget == nullptr)
		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	if (HealthBarWidget&&HealthBarWidget->InGameName)
		HealthBarWidget->InGameName->SetColorAndOpacity(Color);
}

void UHealthBarComponent::AddToHUD()
{
	UWorld* World = GetWorld();
	if (!World) return;
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController) {
		ASoulHUD* SoulHUD = Cast<ASoulHUD>(PlayerController->GetHUD());
		if (SoulHUD) {
			SoulHUD->AddBossBar(HealthBarWidget);
		}
	}
}

void UHealthBarComponent::RemoveFromHUD()
{
	UWorld* World = GetWorld();
	if (!World) return;
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController) {
		ASoulHUD* SoulHUD = Cast<ASoulHUD>(PlayerController->GetHUD());
		if (SoulHUD) {
			SoulHUD->RemoveBossBar(HealthBarWidget);
		}
	}
}
