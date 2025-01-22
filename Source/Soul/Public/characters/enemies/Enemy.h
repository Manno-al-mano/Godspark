// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "characters/BaseCharacter.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "characters/CharacterStates.h"
#include "Enemy.generated.h"

class UAnimMontage;
class UAttributeComponent;
class UHealthBarComponent;
class AAIController;
class UPawnSensingComponent;
class ASoulItem;
class AProjectile;


UCLASS()
class SOUL_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	void GetHit_Implementation(const FVector& ImpactPoint, const FVector& ImpactNormal, const FVector& ActorLocation) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
protected:
	UFUNCTION()
	void HearNoise(APawn* NoiseInstigator, const FVector& Location, float Volume);
	virtual void Attack();
	virtual void JumpAttack();
	void EnemySearch();
	AActor* PatrolArea();
	void StartMovement(AActor* Target);
	void PatrolTargetFinished();
	void HandleDamage(float Damage) override;
	virtual  void PawnSeen(APawn* SeenPawn) override;
	UFUNCTION(BlueprintCallable)
	void EndStagger();
	virtual void Die_Implementation() override;
	void SpawnSoul(UWorld* World);

	void RegenPoise() override;
	virtual void ChangeHealthbarVisibility(bool status);
	virtual void LoseTarget();
	void GoOnPatrol();
	bool IsOutsideCombatRange();
	bool IsOutsideAttackRadius();
	bool IsDead();
	void StartChasing();
	void StartAttackTimer();
	void ClearPatrolTimer();
	void ClearAttackTimer();
	void AttackEnd() override;
	void ChooseHitSide(const FVector& ImpactPoint) override;
	

	FTimerHandle PatrolTimer;

	UFUNCTION(BlueprintCallable)
	void ShootProjectile();
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category = Combat)
	bool bDoesChainAttacks = false;
	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackCooldownMin=.5f;
	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackCooldownMax = 1.5f;

	FTimerHandle AttackTimer;
	UPROPERTY(EditAnywhere)
	UHealthBarComponent* HealthBarComponent;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ai Navigation" )
	TArray<AActor*> PatrolTargets;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ai Navigation")
	AActor* PatrolTargetCurrent;
	UPROPERTY(EditAnywhere, Category = "Combat")
	double CombatRadius = 1000.f;
	UPROPERTY(EditAnywhere, Category = "Combat")
	double AttackRadius = 300.f;
	UPROPERTY(EditAnywhere, Category = "Combat")
	double CloseAttackRadius = 150.f;
	UPROPERTY(EditAnywhere, Category = "Ai Navigation")
	float WaitMin = 3.f;
	UPROPERTY(EditAnywhere, Category = "Ai Navigation")
	float WaitMax = 10.f;
	UPROPERTY(EditAnywhere, Category = "Ai Navigation")
	double PatrolRadius=200.f;

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AMyWeapon> WeaponClass;
	UPROPERTY(EditAnywhere)
	FName SocketName = FName("MainHandSocket");
	UPROPERTY(EditAnywhere)
	FText InGameName= FText::FromString(TEXT("Imaginary Troll"));
	UPROPERTY(EditAnywhere)
	TSubclassOf<ASoulItem> SoulClass;
	AAIController* EnemyController;
	UPROPERTY(EditAnywhere)
	FName RangedSocketName = FName("RangedSocket");
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TArray<FName> RangedMontageSections;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	float RangedAttackSpeed=1.f;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere)
	FColor NameColor;


};
