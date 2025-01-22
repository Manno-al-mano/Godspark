// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractableInterface.h"
#include "Portal.generated.h"

UCLASS()
class SOUL_API APortal : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
public:	
	APortal();
	void Interact(class AMyCharacter* Character) override;
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void ActivatePortal ();
protected:
	UPROPERTY(EditAnywhere, Category = "Portal")
	class UNiagaraComponent* ParticleSystem;
	UPROPERTY(EditAnywhere, Category = "Portal")
	UMeshComponent* PortalMesh;
	UPROPERTY(EditAnywhere, Category = "Portal")
	bool bIsActive = true;
	UPROPERTY(VisibleAnywhere, Category = "Portal")
	class USphereComponent* Sphere;
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UPROPERTY(EditAnywhere, Category = "Portal")
	FName MapName;
	UPROPERTY(EditAnywhere, Category = "Portal")
	FText ActionText;
	FTimerHandle PortalIgnitionTimer;
	virtual void BeginPlay() override;
public:
	FORCEINLINE EInteractableType CheckType() override { return EInteractableType::EIT_Portal; }


};
