// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "MyCameraComponent.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_API UMyCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
	
public:
	UMyCameraComponent();
	UPROPERTY(EditAnywhere)
	class UPawnSensingComponent* PawnSensor;
	
};
