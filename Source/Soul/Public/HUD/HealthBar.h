// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* PoiseBar;
	UPROPERTY(meta = (BindWidget))
	class UImage* PoiseHighlight;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* InGameName;
	
};
