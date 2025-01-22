// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractableInterface.h"
#include "MyItem.generated.h"


class USphereComponent;

enum class EItemState : uint8
{
	EIS_Hover,
	EIS_Equpped,
	EIS_Lie
};

UCLASS()
class SOUL_API AMyItem : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	AMyItem();
	virtual void Tick(float DeltaTime) override;
	virtual bool DoesHaveTag(const FName& Tag) override;
protected:
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintPure)
	float TransformedSin(float Value);
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UPROPERTY(EditAnywhere)
	UMeshComponent* ItemMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speedmult=3.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Amplitude = 0.25f;
	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;
	UPROPERTY(BlueprintReadOnly)
	float ElapsedTime;
	EItemState ItemState = EItemState::EIS_Hover;
	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* ParticleSystem;
	void PlayPickupEffect();
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* PickupEffect;
	void PlayPickUpSound();
	UPROPERTY(EditAnywhere)
	USoundBase* PickupSound;
};
