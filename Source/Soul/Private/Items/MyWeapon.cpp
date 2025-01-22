// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/MyWeapon.h"
#include "characters/MyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Interfaces/HitInterface.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraComponent.h"

AMyWeapon::AMyWeapon() {
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
	BoxComponent->SetupAttachment(GetRootComponent());
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	TraceStart->SetupAttachment(GetRootComponent());

	TraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	TraceEnd->SetupAttachment(GetRootComponent());
}

void AMyWeapon::Interact(AMyCharacter* Character)
{
	if(Character)
	Equip(Character->GetMesh(),FName("MainHandSocket"),Character,Character);
}

void AMyWeapon::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AMyWeapon::OnBoxOverlap);
	
}


void AMyWeapon::Equip(USceneComponent* InParent,const FName& InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	AttachMeshToSocket(InParent, InSocketName);
	ItemState = EItemState::EIS_Equpped;
	PlayEquipSound();
	DisableSphereCollision();
	DisableParticleSystem();
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	IgnoreActors.Add(NewOwner);
}

void AMyWeapon::PlayEquipSound()
{
	if (EquipSound) {
		UGameplayStatics::PlaySoundAtLocation(
			this,
			EquipSound,
			GetActorLocation());

	}
}

void AMyWeapon::DisableParticleSystem()
{
	if (ParticleSystem)
		ParticleSystem->Deactivate();
}

void AMyWeapon::DisableSphereCollision()
{
	if (Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AMyWeapon::SetVisibility(bool bVisible)
{
	ItemMesh->SetVisibility(bVisible);
}

void AMyWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
}




void AMyWeapon::TraceHit(FHitResult& Hit)
{
	auto ToIgnore = TArray<AActor*, FDefaultAllocator>();
	ToIgnore.Add(this);
	ToIgnore.Add(GetOwner());
	for (AActor* Actor : IgnoreActors) {
		ToIgnore.AddUnique(Actor);
	}
	const FVector Start = TraceStart->GetComponentLocation();
	const FVector End = TraceEnd->GetComponentLocation();

	UKismetSystemLibrary::BoxTraceSingle(this,
		Start, End, FVector(TraceSizeX, TraceSizeY, TraceSizeZ),
		TraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ToIgnore,
		EDrawDebugTrace::None,
		Hit,
		true
	);
}

void AMyWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->ActorHasTag(IgnoreTag)) return;
	FHitResult Hit;
	TraceHit(Hit);
	if (!Hit.GetActor()) return;
	if (Hit.GetActor()->ActorHasTag(IgnoreTag)) return;
	HandleUniversalHitEffects(Hit);
	HandleHitInterface(Hit);



}
void AMyWeapon::HandleHitInterface(FHitResult& Hit)
{
	IHitInterface* HitInterface = Cast<IHitInterface>(Hit.GetActor());
	if (HitInterface) {
		//HitInterface->GetHit(Hit.ImpactPoint, Hit.ImpactNormal);
		HitInterface->Execute_GetHit(Hit.GetActor(), Hit.ImpactPoint, Hit.ImpactNormal,GetOwner()->GetActorLocation());
	}
	else {
		HitStaticObject(Hit.ImpactPoint, Hit.ImpactNormal);
	}
}
void AMyWeapon::HandleUniversalHitEffects(FHitResult& Hit)
{
	UGameplayStatics::ApplyDamage(
		Hit.GetActor(),
		Damage,
		GetInstigator()->GetController(),
		this,
		UDamageType::StaticClass());
	IgnoreActors.AddUnique(Hit.GetActor());
	CreateFields(Hit.ImpactPoint);
}
void AMyWeapon::HitStaticObject(const FVector& ImpactPoint, const FVector& ImpactNormal){
	if (StoneHitSound)
		UGameplayStatics::PlaySoundAtLocation(
			this,
			StoneHitSound,
			ImpactPoint);
	if (HitSparks) {
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitSparks,
			ImpactPoint,
			FRotationMatrix::MakeFromZ(ImpactNormal).Rotator(),
			(FVector)(ParticleScale));
	}
}


void AMyWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IInteractInterface* Character = Cast<IInteractInterface>(OtherActor);
	if (Character) {
		Character->SetOverlappingItem(this);
		Character->SetAction(ActionText);
	}
}

void AMyWeapon::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
