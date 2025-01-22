// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "characters/BaseCharacter.h"
#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "characters/CharacterStates.h"
#include "Interfaces/InteractInterface.h"
#include "MyCharacter.generated.h"

class UDialogueComponent;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
class UGroomComponent;
class AMyItem;
class AMyWeapon;
class UPlayerHUD; 
class UPawnNoiseEmitterComponent;
class IInteractableInterface;
class ASoulItem;
class UInventoryWidget;
class ATreasure;



UCLASS()
class SOUL_API AMyCharacter : public ABaseCharacter, public IInteractInterface
{
	GENERATED_BODY()

public:
	AMyCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void AddGold(ATreasure* Count) override;
	
	void AddSouls(ASoulItem* Count) override;
	virtual void Jump() override;
	void SetAction(const FText& Action) override;
	virtual void SetOverlappingItem(IInteractableInterface* MyItem) override;
	void DisablePlayerHUD();
	void EnablePlayerHUD();
	void EquipFromInventory(int InventoryIndex);
	int32 GetGold();
protected:
	
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnMeshCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	void Move(const FInputActionValue& Value);
	void Turn(const FInputActionValue& Value);
	void AttackRotate(const FVector2D& MoveVector);
	void Zoom(const FInputActionValue& Value);
	void ChangeMovementMode();
	void Interact();
	void HoldMovement();
	void RegenPoise() override;
	void ReleaseMovement();
	void HoldSprint();
	void Equip();
	void Attack() override;
	void Dodge();
	void Draw();
	void Block();
	void SkipDialogue();
	void BlockEnd();
	void MenuMove(const FInputActionValue& Value);
	void Die_Implementation() override;
	UFUNCTION(BlueprintCallable)
	void Disarm();
	UFUNCTION(BlueprintCallable)
	void Arm();
	UFUNCTION(BlueprintCallable)
	void SetUnoccupiedState();
	void GetHit_Implementation(const FVector& ImpactPoint, const FVector& ImpactNormal, const FVector& ActorLocation) override;
	void GetHitBlock(const FVector& ImpactPoint);
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	bool HandleBlockDamage(AActor* DamageCauser, float DamageAmount);
	void PlayDrawMontage(const FName& EquipName);
	void AttackEnd() override;
	void TurnOnFocus();
	void TurnOffFocus();
	void Focus();
	void OpenInventory();
	void HandleDamage(float Damage) override;
	void UpdateUnfocusedWarpTarget();
	FVector GetRotationWarpTarget() override;
	FVector GetTranslationWarpTarget() override;
	UPROPERTY(EditAnywhere)
	float StaminaAttackCost = 20.f;
	UPROPERTY(EditAnywhere)
	float StaminaDodgeCost = 30.f;
	UPROPERTY(EditAnywhere)
	float StaminaJumpCost = 10.f;
	FTimerHandle StaminaConsumption;
	FTimerHandle StaminaRegen;
	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);
	//FVector2D MoveVector;
	void RegenStamina();
	UPROPERTY(EditAnywhere)
	float StaminaRegenRate = 25.f;
	float WeaponDamage;
	void ConsumeStaminaSprint();
	void ConsumeStamina(float);
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DodgeMontage;


	APlayerController* PlayerController;
	UPROPERTY(BlueprintReadOnly)
	FVector UnfocusedWarpTarget = FVector();
	void AlignCameraToCharacter();
	void AlignCameraToTarget();
	void SetFocusPoint();
	bool pFocusDone = false;
	virtual  void PawnSeen(APawn* SeenPawn) override;
	FTimerHandle AlignmentTimer;
	UPlayerHUD* PlayerHUD;
	UInventoryWidget* InventoryWidget;
	UPawnNoiseEmitterComponent* NoiseEmitter;
	UPROPERTY(EditDefaultsOnly)
	double FocusRange = 500.f;
	UPROPERTY(BlueprintReadOnly)
	AActor* FocusedTarget;
	class UDialogueComponent* DialogueComponent;
	UPROPERTY(BlueprintReadOnly)
	bool bIsFocusOn = false;
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;
	bool CanRun = true;
	UPROPERTY(EditAnywhere, Category = Movement);
	float SprintSpeed = 900.f;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* SoulContext;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* DodgeAction;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* InventoryAction;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* WalkRunAction;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* WalkAction;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* ZoomAction;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* TurnAction;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* EquipAction;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* DrawAction;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* AttackAction;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* FocusAction;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* BlockAction;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Brows;
	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Hair;
	bool bAttackQueued = false;
	UPROPERTY(BlueprintReadOnly)
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	UPROPERTY(BlueprintReadWrite)
	EActionState ActionState = EActionState::EAS_Unoccupied;
	IInteractableInterface* OverlappingItem;
	/* Animation Montages*/
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* DrawMontage;
	void PlayAttackMontage(const FName& AttackName, float MontageSpeed) override;
	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* BlockSound;
	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* ParrySound;
	bool bBlocked = false;
	bool bParried = false;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere);
	bool bParryWindow = false;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere);
	bool bBlockAnimStarted = false;
	TArray<AMyWeapon*> WeaponsInInventory;
	void UpdatePoise();
	void UpdateStamina();
	void UpdateHealth();

public:
	//FORCEINLINE virtual void SetOverlappingItem(IInteractableInterface* MyItem) override { OverlappingItem = MyItem; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE ECharacterState GetCharacterState() { return CharacterState; }
	FORCEINLINE AMyWeapon* GetEquippedWeapon() { return EquippedWeapon; }
	FORCEINLINE void AddGold(int32 Count);
	void SetDialogueComponent(UDialogueComponent* MenuComponent);

};
