// Fill out your copyright notice in the Description page of Project Settings.


#include "MyComponents/MyCameraComponent.h"
#include "Perception/PawnSensingComponent.h"

UMyCameraComponent::UMyCameraComponent()
{
	UPawnSensingComponent;
	PawnSensor= CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensor"));
}
