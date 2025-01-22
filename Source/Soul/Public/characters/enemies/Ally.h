// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "characters/enemies/Enemy.h"
#include "Interfaces/InteractableInterface.h"
#include "Ally.generated.h"


class UAnimMontage;
class USphereComponent;
class UMercenaryDialogueComponent;
/**
 * 
 */
UCLASS()
class SOUL_API AAlly : public AEnemy, public IInteractableInterface
{
	GENERATED_BODY()
public:
	AAlly();
	void Interact(AMyCharacter* Character) override;
	virtual bool DoesHaveTag(const FName& Tag) override;
	void StartService(AMyCharacter* Character);
protected:
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	void BeginPlay() override;
	void PawnSeen(APawn* SeenPawn) override;
	void LoseTarget() override;
	UFUNCTION(BlueprintCallable)
	void StandUp();
	UPROPERTY(VisibleInstanceOnly)
	bool bHasSeenPlayer = false;
	UPROPERTY(BlueprintReadWrite)
	bool bHasStoodUp=false;
	UPROPERTY(BlueprintReadWrite)
	bool bShouldStandUp = false;
	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;
	UPROPERTY(EditAnywhere)
	FText ActionText;
	UPROPERTY(EditAnywhere)
	UMercenaryDialogueComponent* DialogueComponent;
public:
	FORCEINLINE EInteractableType CheckType() override { return EInteractableType::EIT_Dialogue; }

};
