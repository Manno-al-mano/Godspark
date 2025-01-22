// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SoulHUD.h"
#include "HUD/PlayerHUD.h"
#include "HUD/DialogueWidget.h"
#include "HUD/InventoryWidget.h"
#include "Blueprint/UserWidget.h"
void ASoulHUD::BeginPlay()
{
	Super::BeginPlay();
	UWorld* World = GetWorld();
	if (!World) return;
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!PlayerController) return;
	PlayerHUD = CreateWidget<UPlayerHUD>(PlayerController, PlayerHUDClass);
	if (PlayerHUD)
	PlayerHUD->AddToViewport();
	DialogueWidget = CreateWidget<UDialogueWidget>(PlayerController, DialogueWidgetClass);
	if (DialogueWidget) 
	{
		DialogueWidget->AddToViewport();
		DialogueWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	InventoryWidget = CreateWidget<UInventoryWidget>(PlayerController,InventoryWidgetClass);
	if (InventoryWidget) {
		InventoryWidget->AddToViewport();
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ASoulHUD::AddBossBar(UHealthBar* Widget)
{
	if (Widget)
	{
		BossBar = Widget;
		BossBar->AddToViewport();
		BossBar->SetVisibility(ESlateVisibility::Visible);
	}
}
void ASoulHUD::RemoveBossBar(UHealthBar* Widget)
{
	if (BossBar == Widget)
	{
		
		BossBar->RemoveFromParent();
		BossBar->SetVisibility(ESlateVisibility::Hidden);
		BossBar = nullptr;
	}
}

