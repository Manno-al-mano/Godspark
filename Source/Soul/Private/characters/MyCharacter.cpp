// Fill out your copyright notice in the Description page of Project Settings.


#include "characters/MyCharacter.h"
#include "characters/BaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
//#include "Components/BoxComponent.h"
//#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GroomComponent.h"
#include "Items/SoulItem.h"
#include "Items/Treasure.h"
#include "Items/MyWeapon.h"
#include "MyComponents/AttributeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/SoulHUD.h"
#include "HUD/InventoryWidget.h"
#include "HUD/PlayerHUD.h"
#include "MyComponents/HUDControlComponent.h"
#include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Macros.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "MyComponents/DialogueComponent.h"
#include "Items/Projectile.h"

AMyCharacter::AMyCharacter()
{
	
	PrimaryActorTick.bCanEverTick = true;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 250.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);
	NoiseEmitter = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("NoiseEmitter"));
	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	Brows = CreateDefaultSubobject<UGroomComponent>(TEXT("Brows"));
	Brows->SetupAttachment(GetMesh());
	Brows->AttachmentName = FString("head");

}


// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("PlayerFaction"));
	PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
 ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(SoulContext, 0);
		}
	}
	PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController) {
		ASoulHUD* SoulHUD = Cast<ASoulHUD>(PlayerController->GetHUD());
		if (SoulHUD) {
			PlayerHUD =	SoulHUD->GetPlayerHUD();
			InventoryWidget = SoulHUD->GetInventoryWidget();
		}
	}
	if (PlayerHUD) {
		UpdateHealth();
		UpdatePoise();
		PlayerHUD->SetFocusVisibility(false);
		PlayerHUD->SetGoldCount(0);
		PlayerHUD->SetSoulCount(0);
	}
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AMyCharacter::OnMeshCollision);

	
}
void AMyCharacter::OnMeshCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ActionState != EActionState::EAS_Dodge) return;
	if (Hit.GetActor() && OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_Destructible)
	{
		IHitInterface* HitInterface = Cast<IHitInterface>(Hit.GetActor());
		if (HitInterface) {
			HitInterface->Execute_GetHit(Hit.GetActor(), Hit.ImpactPoint, Hit.ImpactNormal, GetOwner()->GetActorLocation());
			CreateFields(Hit.ImpactPoint);
		}
	}
}
void AMyCharacter::AttackRotate(const FVector2D& MoveVector) {
	{
		if (bIsFocusOn) return;
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// Calculate the target rotation based on input direction relative to camera
		FVector InputDirection = ForwardDirection * MoveVector.Y + RightDirection * MoveVector.X;
		FRotator TargetRotation = InputDirection.Rotation();

		// Smoothly interpolate towards the target rotation
		FRotator CurrentRotation = GetActorRotation();
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 5);

		// Apply the new rotation
		SetActorRotation(NewRotation);
	
	}
}
void AMyCharacter::Zoom(const FInputActionValue& Value)
{
float Zoom = 5*Value.Get<float>();
	if (CameraBoom && CameraBoom->TargetArmLength+Zoom> 50 && CameraBoom->TargetArmLength + Zoom <1000)
		CameraBoom->TargetArmLength += Zoom;
}


void AMyCharacter::ChangeMovementMode()
{
	if (!GetCharacterMovement())
		return;
	CanRun = !CanRun;
	if (CanRun) {
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
	else {
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}

		
}



void AMyCharacter::HoldMovement()
{
	if (!GetCharacterMovement())
		return;
	if (!CanRun) {
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
	else {
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void AMyCharacter::RegenPoise()
{
	if (Attribute && PlayerHUD) {
		Attribute->IncreasePoise();
		float PoisePercent = GetCurrentPoise();
		UpdatePoise();
		if (PoisePercent != 0)
			GetWorldTimerManager().SetTimer(PoiseRegen, this, &AMyCharacter::RegenPoise, LaterRegenTime);
	}
}

void AMyCharacter::ReleaseMovement()
{
	if (!GetCharacterMovement())
		return;
	GetWorldTimerManager().ClearTimer(StaminaConsumption);
	if (CanRun) {
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
	else {
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void AMyCharacter::HoldSprint()
{
	if (GetCharacterMovement()) {
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		ConsumeStaminaSprint();
	}
}
void AMyCharacter::MenuMove(const FInputActionValue& Value) {

	if (DialogueComponent) {
		const FVector2D MoveVector = Value.Get<FVector2D>();
		DialogueComponent->MoveAroundMenu(MoveVector.Y);
	}
	else if (InventoryWidget && InventoryWidget->IsMenuOpen()) {
		const FVector2D MoveVector = Value.Get<FVector2D>();
		InventoryWidget->MoveAroundMenu(MoveVector.Y);
	}
		
}


void AMyCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MoveVector = Value.Get<FVector2D>();
	if (GetCharacterMovement()) {
		float Sound = GetCharacterMovement()->MaxWalkSpeed > WalkSpeed ?
			GetCharacterMovement()->MaxWalkSpeed > RunSpeed ? 1.f : 0.75f : 0.5f;
	NoiseEmitter->MakeNoise(this ,Sound, GetActorLocation());
	}
	if (ActionState != EActionState::EAS_Unoccupied) {
		if (ActionState < EActionState::EAS_AttackingEnd) AttackRotate(MoveVector);
		return;
	}


	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	FVector InputDirection = FVector(MoveVector.X, MoveVector.Y, 0.f).GetSafeNormal();
	
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	
	AddMovementInput(ForwardDirection, MoveVector.Y);
	AddMovementInput(RightDirection, MoveVector.X);
}



void AMyCharacter::Turn(const FInputActionValue& Value)
{
	if (FocusedTarget) return;
	
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}

void AMyCharacter::Interact()
{
	if (DialogueComponent) {
		SkipDialogue();
	}
	else if (InventoryWidget && InventoryWidget->IsMenuOpen())
		InventoryWidget->PressButton();
	else 
		Equip();
}

void AMyCharacter::SkipDialogue()
{
		DialogueComponent->PlayerAction();
}


void AMyCharacter::Equip()
{
	if (ActionState != EActionState::EAS_Unoccupied) return;
	
	if (!OverlappingItem) {
		return;
	}
	if (OverlappingItem->CheckType()==EInteractableType::EIT_Portal) {
		OverlappingItem->Interact(this);
		return;
	}
	if (OverlappingItem->CheckType()==EInteractableType::EIT_Dialogue) {
		OverlappingItem->Interact(this);
		return;
	}
	AMyWeapon* OverlappingWeapon = OverlappingItem->GetIfWeapon();
	if (OverlappingWeapon) {
		if (EquippedWeapon) {
			EquippedWeapon->SetVisibility(false);
			EquippedWeapon = nullptr;
		}
		OverlappingItem->Interact(this);
		//OverlappingWeapon->Interact(this);
		CharacterState = ECharacterState::ECS_Weapon;
		OverlappingItem = nullptr;
		EquippedWeapon = OverlappingWeapon;
		WeaponsInInventory.Add(EquippedWeapon);
		WeaponDamage = EquippedWeapon->GetDamage();
	}
}

void AMyCharacter::AttackEnd()
{
	if (EquippedWeapon) EquippedWeapon->SetDamage(WeaponDamage);
	if (bAttackQueued) {
		bAttackQueued = false;
		if (GetCurrentStamina() == 0 || GetMovementComponent()->IsFalling()) {
			SetUnoccupiedState();
			return;
		}
		ConsumeStamina(StaminaAttackCost);
		UpdateStamina();
		OrderedAttack();

	}
	else {
		bAttackQueued = false;
		SetUnoccupiedState();
	}
}

void AMyCharacter::TurnOnFocus()
{
	if (InTargetRange(CombatTarget,FocusRange)) {
		bIsFocusOn = true;
		FocusedTarget = CombatTarget;
		AlignCameraToTarget();
		if (PlayerHUD)PlayerHUD->SetFocusVisibility(true);

	}
	else
		AlignCameraToCharacter();
}

void AMyCharacter::TurnOffFocus()
{
	if(PlayerHUD)PlayerHUD->SetFocusVisibility(false);
	FocusedTarget = nullptr;
	bIsFocusOn = false;

}

void AMyCharacter::Focus()
{
	switch (bIsFocusOn)
	{
		case false:
		TurnOnFocus();
		break;
		case true:
		TurnOffFocus();
		break;

	}
}

void AMyCharacter::OpenInventory()
{
	if(InventoryWidget)
		if (InventoryWidget->IsMenuOpen()) {
			InventoryWidget->CloseMenu();
			if (ActionState == EActionState::EAS_Dialogue) ActionState = EActionState::EAS_Unoccupied;
		}
		else if (ActionState == EActionState::EAS_Unoccupied) {
			InventoryWidget->OpenMenu(this, WeaponsInInventory);
			ActionState = EActionState::EAS_Dialogue;
		}
}

void AMyCharacter::HandleDamage(float Damage)
{
	
ABaseCharacter::HandleDamage(Damage);
		UpdateHealth();
		UpdatePoise();
		GetWorldTimerManager().SetTimer(PoiseRegen, this, &AMyCharacter::RegenPoise, AfterHitRegenTime);
}

void AMyCharacter::UpdateUnfocusedWarpTarget()
{
	
	UnfocusedWarpTarget = GetActorLocation() + GetActorForwardVector() * 1000;
}

FVector AMyCharacter::GetRotationWarpTarget()
{
		if (FocusedTarget) {
			return FocusedTarget->GetActorLocation();
		}
		return GetActorLocation();

}
FVector AMyCharacter::GetTranslationWarpTarget()
{
	if (FocusedTarget == nullptr)
	{
		return FVector();
	}
	const FVector TargetLocation = FocusedTarget->GetActorLocation();
	const FVector Location = GetActorLocation();
	FVector TargetToActor = (Location - TargetLocation).GetSafeNormal();
	TargetToActor *= WarpTargetDistance;
	return TargetLocation + TargetToActor;
}

void AMyCharacter::RegenStamina()
{
	if (!Attribute) return;
	Attribute->IncreaseStamina(
		ActionState == EActionState::EAS_Blocking ? 
		StaminaRegenRate / 1000.f 
		:StaminaRegenRate / 100.f);
	UpdateStamina();
	FString StaminaText = FString::Printf(TEXT("Stamina: %.2f"), GetCurrentStamina());
	if (!Attribute->IsStaminaFull())
		GetWorldTimerManager().SetTimer(StaminaRegen, this, &AMyCharacter::RegenStamina, .01f);

}

void AMyCharacter::ConsumeStaminaSprint()
{
	if (ActionState != EActionState::EAS_Unoccupied||!Attribute) return;
	if (GetMovementComponent()->Velocity != FVector(0)&&!GetMovementComponent()->IsFalling()) {
		ConsumeStamina(.05f);
		if (PlayerHUD)PlayerHUD->SetStaminaPercentFast(GetCurrentStamina());
	}
	GetWorldTimerManager().SetTimer(StaminaConsumption, this, &AMyCharacter::ConsumeStaminaSprint, .01f);
}

void AMyCharacter::ConsumeStamina(float StaminaConsumed)
{
	if (!Attribute) return;

	if (Attribute->DecreaseStamina(StaminaConsumed)) {
		GetWorldTimerManager().SetTimer(StaminaRegen, this, &AMyCharacter::RegenStamina, 3.f);
		ReleaseMovement();
		if (ActionState == EActionState::EAS_Blocking) {
			ActionState = EActionState::EAS_Unoccupied;
		}
	}
	else
	GetWorldTimerManager().SetTimer(StaminaRegen, this, &AMyCharacter::RegenStamina, 1.f);
}

void AMyCharacter::AlignCameraToCharacter()
{
	FRotator CharacterRotation = GetActorRotation();
	FRotator CurrentControlRotation = Controller->GetControlRotation();

	float InterpSpeed = 15.0f; 
	FRotator NewControlRotation = FMath::RInterpTo(CurrentControlRotation, CharacterRotation, GetWorld()->GetDeltaSeconds(), InterpSpeed);

	Controller->SetControlRotation(NewControlRotation);


	if (FMath::IsNearlyEqual(CurrentControlRotation.Yaw, CharacterRotation.Yaw, 1.0f))
	{
		return;
	}
	GetWorld()->GetTimerManager().SetTimer(AlignmentTimer, this, &AMyCharacter::AlignCameraToCharacter, GetWorld()->GetDeltaSeconds(), false);
}

void AMyCharacter::AlignCameraToTarget()
{
	if (FocusedTarget)
	{
		FRotator TargetRotation = (FocusedTarget->GetActorLocation() - GetActorLocation()).Rotation();
		FRotator CurrentControlRotation = Controller->GetControlRotation();
		TargetRotation.Pitch -= 25;
		TargetRotation.Roll = CurrentControlRotation.Roll;

		float InterpSpeed = 10.0f;
		FRotator NewControlRotation = FMath::RInterpTo(CurrentControlRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), InterpSpeed);

		Controller->SetControlRotation(NewControlRotation);
		if (FMath::IsNearlyEqual(CurrentControlRotation.Yaw, TargetRotation.Yaw, .1f))
		{
			GetWorld()->GetTimerManager().ClearTimer(AlignmentTimer);
		}
		else
		{
			GetWorld()->GetTimerManager().SetTimer(AlignmentTimer, this, &AMyCharacter::AlignCameraToTarget, GetWorld()->GetDeltaSeconds()/10, false);
		}
	}
}

void AMyCharacter::SetFocusPoint()
{
	if (PlayerController && PlayerHUD) {
		FVector Center = FocusedTarget->GetActorLocation();
		FVector2D ScreenPosition;
		if (UGameplayStatics::ProjectWorldToScreen(PlayerController, Center, ScreenPosition,true))
			PlayerHUD->SetFocusLocation(ScreenPosition);
	}
}

void AMyCharacter::PawnSeen(APawn* SeenPawn)
{
	bool bCanBeFocused = SeenPawn->ActorHasTag(FName("EnemyFaction"))
		&& !SeenPawn->ActorHasTag(FName("Dead"));
	if (bCanBeFocused)
		CombatTarget = SeenPawn;
}

void AMyCharacter::Attack()
{
	if (AttackMontageSections.Num() == 0) return;
	if (CharacterState == ECharacterState::ECS_Unequipped) {
		if (EquippedWeapon)
			Draw();
		return;
	}
	if (bAttackQueued)
		return;

	if (ActionState == EActionState::EAS_Unoccupied) {
		if (GetCurrentStamina() == 0) return;
		ResetAttackIterator();
		if (GetCharacterMovement()->MaxWalkSpeed == SprintSpeed||GetMovementComponent()->IsFalling())
		{
			NoiseEmitter->MakeNoise(this, 1.f, GetActorLocation()); 
			EquippedWeapon->SetDamage(1.5f*WeaponDamage);
			PlayAttackMontage(FName("JumpAttack"), AttackMontageSpeed);
			ConsumeStamina(StaminaAttackCost*1.5f);
			UpdateStamina();
		}
		else
		{
			ConsumeStamina(StaminaAttackCost);
			
			UpdateStamina();
			OrderedAttack();
			UpdateUnfocusedWarpTarget();
		}

		
	}
	else if (ActionState == EActionState::EAS_AttackingEnd) {
		bAttackQueued = true;
	}
}



void AMyCharacter::PlayAttackMontage(const FName& AttackName, float MontageSpeed)
{
	ABaseCharacter::PlayAttackMontage(AttackName, MontageSpeed);
	ActionState = EActionState::EAS_AttackingStart;
}

void AMyCharacter::SetOverlappingItem(IInteractableInterface* MyItem)
{
	OverlappingItem = MyItem;
	if (OverlappingItem == nullptr)
		if (PlayerHUD)PlayerHUD->DisableActionText();
}

void AMyCharacter::DisablePlayerHUD()
{
	if (PlayerHUD)PlayerHUD->SetVisibility(ESlateVisibility::Hidden);
}

void AMyCharacter::EnablePlayerHUD()
{
	if (PlayerHUD)PlayerHUD->SetVisibility(ESlateVisibility::Visible);
}

void AMyCharacter::EquipFromInventory(int InventoryIndex)
{
	if (InventoryIndex >= 0 && InventoryIndex < WeaponsInInventory.Num()) {
		EquippedWeapon->SetVisibility(false);
		EquippedWeapon->SetDamage(WeaponDamage);
		EquippedWeapon = WeaponsInInventory[InventoryIndex];
		EquippedWeapon->SetVisibility(true);
		EquippedWeapon->Interact(this);
		CharacterState = ECharacterState::ECS_Weapon;
		WeaponDamage = EquippedWeapon->GetDamage();
	}
}

int32 AMyCharacter::GetGold()
{
	if (Attribute)return Attribute->GetGoldCount();
	return 0;
}



void AMyCharacter::PlayDrawMontage(const FName& EquipName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DrawMontage) {
		AnimInstance->Montage_Play(DrawMontage, 1.2f);
		AnimInstance->Montage_JumpToSection(EquipName);
	}
}

void AMyCharacter::Dodge()
{
	//GetLastMovementInputVector();
	if (ActionState != EActionState::EAS_Unoccupied || GetCurrentStamina() == 0 || GetCharacterMovement()->IsFalling()) return;
		ConsumeStamina(StaminaDodgeCost);
		FVector LastMovementInputVector = GetLastMovementInputVector();
		if (!LastMovementInputVector.IsNearlyZero())
		{
			// Calculate rotation based on movement input vector
			FRotator NewRotation = LastMovementInputVector.Rotation();
			SetActorRotation(FRotator(0.f, NewRotation.Yaw, 0.f));
		}
		UpdateStamina();
		ActionState = EActionState::EAS_Dodge;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && DodgeMontage) {
			AnimInstance->Montage_Play(DodgeMontage, 1);
			AnimInstance->Montage_JumpToSection("Dodge");
		}
	
}

void AMyCharacter::Draw()
{
	if (CharacterState == ECharacterState::ECS_Weapon && ActionState == EActionState::EAS_Unoccupied &&DrawMontage)
	{
		PlayDrawMontage(FName("Disarm"));
		CharacterState = ECharacterState::ECS_Unequipped;
		ActionState = EActionState::EAS_Drawing;
	}
	else if (CharacterState == ECharacterState::ECS_Unequipped && ActionState == EActionState::EAS_Unoccupied && DrawMontage&&EquippedWeapon)
	{
		PlayDrawMontage(FName("Equip"));
		CharacterState = ECharacterState::ECS_Weapon;
		ActionState = EActionState::EAS_Drawing;
	}
}

void AMyCharacter::Block()
{
	if (ActionState == EActionState::EAS_Unoccupied &&
		CharacterState == ECharacterState::ECS_Weapon &&
		GetCurrentStamina() > 0) {
		ActionState = EActionState::EAS_Blocking;
		bParryWindow = true;
	}
}



void AMyCharacter::BlockEnd()
{
	if (ActionState == EActionState::EAS_Blocking) {
		ActionState = EActionState::EAS_Unoccupied;
		GetWorldTimerManager().SetTimer(StaminaRegen, this, &AMyCharacter::RegenStamina, 1.f);
	}
}

void AMyCharacter::Die_Implementation()
{
	ABaseCharacter::Die_Implementation();
	ActionState = EActionState::EAS_Dead;
}


void AMyCharacter::Disarm()
{
	if (EquippedWeapon) {
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void AMyCharacter::Arm()
{
	if (EquippedWeapon) {
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("MainHandSocket"));	
	}
}

void AMyCharacter::SetUnoccupiedState()
{
	ActionState = EActionState::EAS_Unoccupied;
	bAttackQueued = false;
}

//void AMyCharacter::EndDodgeRotation()
//{
//	ActionState = EActionState::EAS_DodgeEnd;
//}

void AMyCharacter::GetHit_Implementation(const FVector& ImpactPoint, const FVector& ImpactNormal, const FVector& ActorLocation)
{
	if (!Attribute) return;
	if (!PlayerHUD) return;
	if (bBlocked) {
		return GetHitBlock(ImpactPoint);
	}

	if (Attribute->CalculatePoise() == 0.f)
	{
		ActionState = EActionState::EAS_Staggered;
		SetWeaponCollision(ECollisionEnabled::NoCollision);
		if(EquippedWeapon)EquippedWeapon->SetDamage(WeaponDamage);
	}
	if (Attribute->CalculatePoise() == 1.f)
	{
		if (BreakSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				BreakSound,
				ImpactPoint);
		}
	}
	ABaseCharacter::GetHit_Implementation(ImpactPoint, ImpactNormal, ActorLocation);


}

void AMyCharacter::GetHitBlock(const FVector& ImpactPoint)
{
	if (!isAlive())
	{
		Die();
		return;
	}
	if (bParried && ParrySound) {
		UGameplayStatics::PlaySoundAtLocation(
			this,
			ParrySound,
			ImpactPoint);
	}
	else if (BlockSound)
		UGameplayStatics::PlaySoundAtLocation(
			this,
			BlockSound,
			ImpactPoint);
	if (Attribute->CalculatePoise() == 1.f)
	{
		if (BreakSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				BreakSound,
				ImpactPoint);
		}
	}


	return;
}

float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!Attribute) return DamageAmount;
	if(!PlayerHUD) return DamageAmount;
	if (ActionState == EActionState::EAS_Dialogue) ActionState = EActionState::EAS_Unoccupied;
	if (DialogueComponent) DialogueComponent->EndMenu();
	if (InventoryWidget && InventoryWidget->IsMenuOpen()) InventoryWidget->CloseMenu();
	if(!HandleBlockDamage(DamageCauser, DamageAmount))
	{
		HandleDamage(DamageAmount);
		bBlocked = false;
		bParried = false;
	}
	return DamageAmount;
}

bool AMyCharacter::HandleBlockDamage(AActor* DamageCauser, float DamageAmount)
{
	double HitAngle = CalculateHitAngle(DamageCauser->GetOwner()->GetActorLocation());
	if (ActionState == EActionState::EAS_Blocking && HitAngle<45 && HitAngle>-45) {
		bParried = false;
		bBlocked = false;
		if (bParryWindow && bBlockAnimStarted) {
			if (!DamageCauser->ActorHasTag("Projectile")) {
				UGameplayStatics::ApplyDamage(
					DamageCauser->GetOwner(),
					.1f,
					GetInstigator()->GetController(),
					this,
					UDamageType::StaticClass());
			}
				bBlocked = true;
				bParried = true;
				return true;
			
		}
		ConsumeStamina(DamageAmount / 4);
		bBlocked = true;
		Attribute->DamageHealth(DamageAmount / 2);
		UpdateHealth();
		UpdateStamina();
		GetWorldTimerManager().SetTimer(StaminaRegen, this, &AMyCharacter::RegenStamina, 1.f);
		if (GetCurrentStamina() == 0) {
			Attribute->FullyDamagePoise();
			UpdatePoise();
			GetWorldTimerManager().SetTimer(PoiseRegen, this, &AMyCharacter::RegenPoise, AfterHitRegenTime);
		}
		return true;
	}
	return false;
}


void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!InTargetRange(FocusedTarget, FocusRange)||FocusedTarget->ActorHasTag("Dead")) {
		TurnOffFocus();
	}
	else if (!GetWorldTimerManager().IsTimerActive(AlignmentTimer)) {
		AlignCameraToTarget();
		//FVector TargetDirection = FocusedTarget->GetActorLocation() - GetActorLocation();
		//
		//
		//
		//FRotator OriginalRotation = Controller->GetControlRotation();
		//
		//Controller->SetControlRotation(FRotator(OriginalRotation.Pitch, TargetDirection.Rotation().Yaw, OriginalRotation.Roll));

	}
	if (FocusedTarget) {
		SetFocusPoint();
		if (ActionState==EActionState::EAS_Blocking) {
			FRotator TargetRotation = (FocusedTarget->GetActorLocation()- GetActorLocation()).Rotation();
			FRotator CurrentRotation = GetActorRotation();
			FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 5);
			SetActorRotation(NewRotation);
		}
	}
	

}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &AMyCharacter::MenuMove);
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AMyCharacter::Zoom);
		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &AMyCharacter::Turn);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMyCharacter::Jump);
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &AMyCharacter::OpenInventory);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Started, this, &AMyCharacter::Interact);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AMyCharacter::Attack);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &AMyCharacter::Dodge);
		EnhancedInputComponent->BindAction(DrawAction, ETriggerEvent::Started, this, &AMyCharacter::Draw);
		EnhancedInputComponent->BindAction(WalkRunAction, ETriggerEvent::Started, this, &AMyCharacter::ChangeMovementMode);
		EnhancedInputComponent->BindAction(WalkAction, ETriggerEvent::Triggered, this, &AMyCharacter::HoldMovement);
		EnhancedInputComponent->BindAction(WalkAction, ETriggerEvent::Completed, this, &AMyCharacter::ReleaseMovement);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AMyCharacter::HoldSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AMyCharacter::ReleaseMovement);
		EnhancedInputComponent->BindAction(FocusAction, ETriggerEvent::Started, this, &AMyCharacter::Focus);
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Completed, this, &AMyCharacter::BlockEnd);
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Started, this, &AMyCharacter::Block);
	}

}

void AMyCharacter::AddGold(ATreasure* Treasure)
{
	if (Attribute && Treasure) {
		Attribute->ChangeGoldCount(Treasure->GetGold());
		if (PlayerHUD)PlayerHUD->SetGoldCount(Attribute->GetGoldCount());
	}
}

void AMyCharacter::AddSouls(ASoulItem* Soul)
{
	if (Attribute && Soul) {
		Attribute->ChangeSoulCount(Soul->GetSoulCount());
		if (PlayerHUD)PlayerHUD->SetSoulCount(Attribute->GetSoulCount());
	}
}

void AMyCharacter::Jump()
{
	if (ActionState == EActionState::EAS_Unoccupied && GetCurrentStamina() != 0&&!GetMovementComponent()->IsFalling()) {
		ConsumeStamina(StaminaJumpCost);
		UpdateStamina();
			Super::Jump();
	}
}

void AMyCharacter::SetAction(const FText& Action){
	if (PlayerHUD) PlayerHUD->SetActionText(Action);
}

void AMyCharacter::UpdatePoise() { if (PlayerHUD) PlayerHUD->SetPoisePercent(GetCurrentPoise()); }
void AMyCharacter::SetDialogueComponent(UDialogueComponent* MenuComponent)
{
	DialogueComponent = MenuComponent;
	if (DialogueComponent) {
		ActionState = EActionState::EAS_Dialogue;
	}
	else ActionState = EActionState::EAS_Unoccupied;

}
void AMyCharacter::AddGold(int32 Count)
{
	if (Attribute)Attribute->ChangeGoldCount(Count);
	if (PlayerHUD)PlayerHUD->SetGoldCount(Attribute->GetGoldCount());
}
void AMyCharacter::UpdateStamina() { 
	if (PlayerHUD) PlayerHUD->SetStaminaPercent(GetCurrentStamina());
}
void AMyCharacter::UpdateHealth() { if (PlayerHUD) PlayerHUD->SetHealthPercent(GetCurrentHealth()); }

