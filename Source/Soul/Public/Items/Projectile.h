// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"


UCLASS()
class SOUL_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();
	virtual void Tick(float DeltaTime) override;
	void SetTarget(AActor* ProjectileTarget);
	//void Reflect(APawn* Reflector, AActor* Weapon);
protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	class USphereComponent* SphereComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	class UProjectileMovementComponent* ProjectileMovementComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float DamageValue;
	FName TargetFaction=FName("PlayerFaction");
	bool bReflected = false;
	UPROPERTY(EditDefaultsOnly,Category = "Projectile")
	class UParticleSystem* HitSparks;
	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	USoundBase* HitSound;
};
