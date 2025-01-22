// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "BreakableActor.generated.h"


class UGeometryCollectionComponent;
class UCapsuleComponent;
UCLASS()
class SOUL_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	ABreakableActor();
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION(BlueprintCallable)
	void SpawnGold(const FChaosBreakEvent& BreakEvent);
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UGeometryCollectionComponent* GeometryCollection;
	void GetHit_Implementation(const FVector& ImpactPoint, const FVector& ImpactNormal, const FVector& ActorLocation) override;
	UPROPERTY(EditAnywhere, Category = Sounds, BlueprintReadOnly)
	USoundBase* HitSound;
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class ATreasure>> Treasure;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCapsuleComponent* Capsule;
	bool bBroken = false;
	UPROPERTY(BlueprintReadOnly)
	bool bSpawnedGold = false;
public:	
	

};
