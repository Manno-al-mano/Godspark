// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/MyItem.h"
#include "MyWeapon.generated.h"



class UBoxComponent;
/**
 * 
 */
UCLASS()
class SOUL_API AMyWeapon : public AMyItem
{
	GENERATED_BODY()

public:	
	AMyWeapon();
	virtual void Interact(class AMyCharacter* Character) override;
	void Equip(USceneComponent* InParent, const FName& InSocketName, AActor* NewOwner, APawn* NewInstigator);
	void PlayEquipSound();
	void DisableParticleSystem();
	void DisableSphereCollision();
	UFUNCTION(BlueprintCallable)
	void SetVisibility(bool bVisible);
	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);
	TArray<AActor*> IgnoreActors;

	
protected:
	
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintImplementableEvent) 
	void CreateFields(const FVector& FieldLocation);
	void TraceHit(FHitResult& Hit);

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void HandleHitInterface(FHitResult& Hit);
	void HandleUniversalHitEffects(FHitResult& Hit);
	//UFUNCTION()
	//void OnBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	void HitStaticObject(const FVector& ImpactPoint, const FVector& ImpactNormal);

	
	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* EquipSound;
	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* StoneHitSound;
	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxComponent;
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	USceneComponent* TraceStart;
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	USceneComponent* TraceEnd;
	UPROPERTY(EditAnywhere, Category = VisualEffects)
	UParticleSystem* HitSparks;
	UPROPERTY(EditAnywhere, Category = VisualEffects)
	float ParticleScale=0.5f;
	UPROPERTY(EditAnywhere,Category= Mechanics)
	float Damage=1.f;
	UPROPERTY(EditAnywhere, Category = Mechanics)
	float AttackSpeed = 1.f;
	UPROPERTY(EditAnywhere,Category = Tracing)
	float TraceSizeX = 5.f;
	UPROPERTY(EditAnywhere,Category = Tracing)
	float TraceSizeY = 5.f;
	UPROPERTY(EditAnywhere,Category = Tracing)
	float TraceSizeZ = 5.f;
	UPROPERTY(EditAnywhere, Category = Tracing)
	FName IgnoreTag;
	UPROPERTY(EditAnywhere, Category = Texts)
	FText ActionText;
	UPROPERTY(EditAnywhere, Category = Texts)
	FText ItemName;
	UPROPERTY(EditAnywhere)	
	FText Description;
public:
	FORCEINLINE FText GetItemName() const { return ItemName; }
	FORCEINLINE UBoxComponent* GetBoxComponent() const { return BoxComponent; }
	FORCEINLINE float GetAttackSpeed() const { return AttackSpeed; }
	FORCEINLINE float GetDamage() const { return Damage; }
	FORCEINLINE void SetDamage(float NewDamage) { Damage=NewDamage; }
	FORCEINLINE AMyWeapon* GetIfWeapon() override { return this; };
	FORCEINLINE FText GetDescription() const { return Description; }
	FORCEINLINE EInteractableType CheckType() override { return EInteractableType::EIT_Weapon; }
};
