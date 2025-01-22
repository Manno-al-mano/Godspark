// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/MyItem.h"
#include "characters/MyCharacter.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Interfaces/InteractInterface.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AMyItem::AMyItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	RootComponent = ItemMesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());

	ParticleSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Sparks"));
	ParticleSystem->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AMyItem::BeginPlay()
{
	AActor::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AMyItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AMyItem::OnSphereOverlapEnd);
}

float AMyItem::TransformedSin(float Value)
{
	return Amplitude * FMath::Sin(Value * Speedmult);
}

void AMyItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IInteractInterface* Character = Cast<IInteractInterface>(OtherActor);
	if (Character) {
		Character->SetOverlappingItem(this);
	}
}

void AMyItem::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IInteractInterface* Character = Cast<IInteractInterface>(OtherActor);
	if (Character) {
		Character->SetOverlappingItem(nullptr);
	}
}

void AMyItem::PlayPickupEffect()
{
	if (PickupEffect) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, PickupEffect, GetActorLocation());
	}
}

void AMyItem::PlayPickUpSound()
{
	if (PickupSound) {
		UGameplayStatics::PlaySoundAtLocation(
			this,
			PickupSound,
			GetActorLocation());
	}
}




// Called every frame
void AMyItem::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);
	ElapsedTime += DeltaTime;

	if (ItemState == EItemState::EIS_Hover) {
		AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin(ElapsedTime)));
		AddActorWorldRotation(FRotator(0, DeltaTime*60,0));

	}


}

bool AMyItem::DoesHaveTag(const FName& Tag)
{
	if(GetOwner())
	return GetOwner()->ActorHasTag(Tag);
	return false;
}

