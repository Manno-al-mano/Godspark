// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SOUL_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void DamagePoise();
	bool DecreaseStamina(float Amount);
	
protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category= Attributes)
	float Health=1000.f;
	UPROPERTY(EditAnywhere, Category= Attributes)
	float MaxHealth=1000.f;
	UPROPERTY(EditAnywhere, Category = Attributes)
	float Poise = 3.f;
	UPROPERTY(EditAnywhere, Category = Attributes)
	float MaxStamina = 150.f;
	UPROPERTY(EditAnywhere, Category = Attributes)
	float Stamina = 150.f;
	float HitCount=0.f;
	UPROPERTY(EditAnywhere, Category = Attributes)
	int32 SoulCount=0;
	UPROPERTY(EditAnywhere, Category = Attributes)
	int32 GoldCount = 0;
public:		
	FORCEINLINE void ChangeSoulCount(int32 Amount) { SoulCount += Amount; }
	FORCEINLINE void ChangeGoldCount(int32 Amount) { GoldCount += Amount; }
	FORCEINLINE int32 GetGoldCount() const { return GoldCount; }
	FORCEINLINE int32 GetSoulCount() const { return SoulCount; }
	FORCEINLINE float CalculateHealth() const{ return Health / MaxHealth; }
	FORCEINLINE float CalculatePoise()const { return HitCount / Poise;}
	FORCEINLINE float CalculateStamina() const{ return Stamina / MaxStamina; }
	FORCEINLINE bool IsAlive()const { return Health > 0; }
	FORCEINLINE void FullyDamagePoise() { HitCount = Poise; }
	FORCEINLINE void IncreasePoise() { if (HitCount > 0) HitCount--; }
	FORCEINLINE void IncreaseStamina(float Amount) { Stamina = FMath::Clamp(Stamina + Amount, 0.0f, MaxStamina); }
	FORCEINLINE void DamageHealth(float Dmg) { Health = FMath::Clamp(Health - Dmg, 0.f, MaxHealth); }
	FORCEINLINE bool IsStaminaFull() const { return Stamina >= MaxStamina; }


};
