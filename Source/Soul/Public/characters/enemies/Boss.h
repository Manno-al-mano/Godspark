// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "characters/enemies/Enemy.h"
#include "Boss.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_API ABoss : public AEnemy
{
	GENERATED_BODY()
protected:
	virtual  void PawnSeen(APawn* SeenPawn) override;
	void ChangeHealthbarVisibility(bool status) override;
	virtual void Die_Implementation() override;


};
