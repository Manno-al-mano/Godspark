// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/HitInterface.h"
#include "GameFramework/Character.h"
#include "characters/CharacterStates.h"
#include "BaseCharacter.generated.h"

class AMyWeapon;
class UPawnSensingComponent;
class UAttributeComponent;
UCLASS()
class SOUL_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, const FVector& ImpactNormal, const FVector& ActorLocation) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;



protected:

	UFUNCTION(BlueprintCallable)
	virtual void SetWeaponCollision(ECollisionEnabled::Type CollisionType);
	virtual double CalculateHitAngle(const FVector& ImpactPoint);
	virtual void ChooseHitSide(const FVector& ImpactPoint);
	virtual bool InTargetRange(AActor* Target, double Range);
	virtual void RegenPoise();
	UFUNCTION()
	virtual  void PawnSeen(APawn* SeenPawn);
	UFUNCTION(BlueprintNativeEvent)
	void Die();
	virtual void Attack();
	UPROPERTY(EditAnywhere, Category = "Combat")
	double WarpTargetDistance = 75.f;
	UFUNCTION(BlueprintCallable)
	virtual FVector GetTranslationWarpTarget();
	UFUNCTION(BlueprintCallable)
	virtual FVector GetRotationWarpTarget();
	virtual void PlayHitReactMontage(const FName& Name);
	virtual void PlayDeathReactMontage(const FName& Name);
	virtual void BeginPlay() override;
	virtual void PlayAttackMontage(const FName& AttackName, float MontageSpeed);
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();
	virtual void HandleDamage(float Damage);
	bool isAlive();
	void RandomAttack();
	void OrderedAttack();
	void ResetAttackIterator();
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	float AttackMontageSpeed=1;
	UPROPERTY(EditDefaultsOnly, Category = Combat)
	float PoiseBreakMultiplier = 1.5f;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TArray<FName> DeathMontageSections;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TArray<FName> AttackMontageSections;
	int8 AttackIterator = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Ai Navigation")
	AActor* CombatTarget;
	UPROPERTY(EditAnywhere, Category = VisualEffects)
	UParticleSystem* HitParticles;
	UPROPERTY(EditAnywhere, Category = VisualEffects)
	float ParticleScale = 0.5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement);
	float WalkSpeed = 300.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement);
	float RunSpeed = 600.f;
	UPROPERTY(EditAnywhere)
	float AfterHitRegenTime = 5.f;
	UPROPERTY(EditAnywhere)
	float LaterRegenTime = 0.5f;
	FTimerHandle PoiseRegen;
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AMyWeapon* EquippedWeapon;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* ReactToHitMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DeathMontage;
	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* HitSound;
	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* BreakSound;
	UPROPERTY(EditAnywhere)
	UAttributeComponent* Attribute;
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly)
	TEnumAsByte<EDeathPose> DeathPose;
	UPROPERTY(EditAnywhere)
	UPawnSensingComponent* PawnSensor;
	float GetCurrentStamina();
	float GetCurrentHealth();
	float GetCurrentPoise();
};
