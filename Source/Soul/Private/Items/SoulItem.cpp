// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/SoulItem.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "characters/MyCharacter.h"

void ASoulItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bGroundReached) return;
	if (GetActorLocation().Z > DesiredLocation)
		AddActorWorldOffset(FVector(0.f, 0.f, -FallSpeed * DeltaTime));
	else bGroundReached = true;
}

void ASoulItem::BeginPlay()
{
	Super::BeginPlay();
	const FVector EndPoint = GetActorLocation() - FVector(0.f, 0.f, 2000);
	FHitResult Hit;
	UKismetSystemLibrary::LineTraceSingle(this,
		GetActorLocation(),
		EndPoint,
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::None,
		Hit,
		true);
	DesiredLocation = Hit.ImpactPoint.Z + 75.f;
	
	
}

void ASoulItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IInteractInterface* Character = Cast<IInteractInterface>(OtherActor);
	if (Character&&!bPickedUp) {
		Character->AddSouls(this);
		ParticleSystem->Deactivate();
		SetLifeSpan(.35f);
		bPickedUp = true;
		PlayPickupEffect();
		PlayPickUpSound();
	}
}

