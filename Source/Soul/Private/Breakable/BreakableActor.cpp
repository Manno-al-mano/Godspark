// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakable/BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Items/Treasure.h"

// Sets default values
ABreakableActor::ABreakableActor()
{
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Geometry Collection"));
	SetRootComponent(GeometryCollection);
	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GeometryCollection->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GeometryCollection->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	Capsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	Capsule->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	Capsule->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
	GeometryCollection->OnChaosBreakEvent.AddDynamic(this, &ABreakableActor::SpawnGold);
	
}
void ABreakableActor::SpawnGold(const FChaosBreakEvent& BreakEvent) {
	UWorld* World = GetWorld();
	if (!World)
		return;
	if (bSpawnedGold)
		return;
	bSpawnedGold = true;
	FHitResult HitResult;
	FCollisionQueryParams TraceParams(FName(TEXT("GroundTrace")), true, this);
	FVector Start = GetActorLocation();
	Start += FVector(0, 0, 100);
	FVector End = Start - FVector(0, 0, 1000);
	const int32 Rand = FMath::RandRange(1, 30);
	if ((Rand <= 10 || Treasure.Num() != 3) && World->LineTraceSingleByChannel(HitResult, Start, End, ECC_WorldStatic, TraceParams))
	{

		// Trace hit something
		FVector HitLocation = HitResult.ImpactPoint;

		if (Treasure.Num() == 3) {
			if (Rand < 6)
				World->SpawnActor<ATreasure>(Treasure[0], HitLocation, FRotator(0));
			else if (Rand < 10)
				World->SpawnActor<ATreasure>(Treasure[1], HitLocation, FRotator(0));
			else if(Rand ==10)
				World->SpawnActor<ATreasure>(Treasure[2], HitLocation, FRotator(0));

		}
		else if (Treasure.Num() == 1)
			World->SpawnActor<ATreasure>(Treasure[0], HitLocation, FRotator(0));
		else if (Treasure.Num() == 2) {
			if (Rand <= 20)
				World->SpawnActor<ATreasure>(Treasure[0], HitLocation, FRotator(0));
			else
				World->SpawnActor<ATreasure>(Treasure[1], HitLocation, FRotator(0));
		}
			
	}
}

void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint, const FVector& ImpactNormal, const FVector& ActorLocation)
{
	if (bBroken)
		return;
	bBroken = true;

	if (HitSound)
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint);

	
}

// Called every frame
void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

