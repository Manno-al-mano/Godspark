// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/MyItem.h"
#include "SoulItem.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_API ASoulItem : public AMyItem
{
	GENERATED_BODY()
	void Tick(float DeltaTime) override;
	void BeginPlay() override;


protected:
	bool bGroundReached=false;
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	bool bPickedUp = false;
	UPROPERTY(EditAnywhere)
	int32 SoulCount = 100;
	UPROPERTY(VisibleAnywhere)
	float DesiredLocation = 0;
	UPROPERTY(EditAnywhere)
	float FallSpeed = 20;

public:
	FORCEINLINE int32 GetSoulCount() const { return SoulCount; }
	FORCEINLINE void SetSoulCount(int32 Count) { SoulCount = Count; }
};
