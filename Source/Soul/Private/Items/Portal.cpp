// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Portal.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/InteractInterface.h"
#include "components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Macros.h"
// Sets default values
APortal::APortal()
{
	
	Sphere = CreateDefaultSubobject<USphereComponent>(FName("Sphere"));
	RootComponent = Sphere;
	PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Portal Mesh"));
	PortalMesh->SetupAttachment(GetRootComponent());
	ParticleSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Portal Particles"));
	ParticleSystem->SetupAttachment(GetRootComponent());
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	

}

void APortal::Interact(AMyCharacter* Character)
{
	
	UGameplayStatics::OpenLevel(GetWorld(), MapName, true);
}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	if (!bIsActive) {
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ParticleSystem->Deactivate();
		FVector CurrentScale = PortalMesh->GetRelativeScale3D();
		FVector NewScale = FVector(CurrentScale.X,.05f , .05f);
		PortalMesh->SetRelativeScale3D(NewScale);
		//ActivatePortal();
	}
	Tags.Add("PortalClass");
	AActor::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &APortal::OnSphereOverlapEnd);
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

}

void APortal::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IInteractInterface* Character = Cast<IInteractInterface>(OtherActor);
	if (Character) {
		Character->SetOverlappingItem(this);
		Character->SetAction(ActionText);
	}
}

void APortal::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IInteractInterface* Character = Cast<IInteractInterface>(OtherActor);
	if (Character) {
		Character->SetOverlappingItem(nullptr);
	}
}

	void APortal::ActivatePortal()
	{
		// Check if the current Y and Z scales are less than 3.6
		FVector CurrentScale = PortalMesh->GetRelativeScale3D();
		if (CurrentScale.Y < 3.6f && CurrentScale.Z < 3.6f)
		{
			// Increase Y and Z scales by 0.05
			FVector NewScale = FVector(CurrentScale.X, FMath::Min(CurrentScale.Y + 0.01f, 3.6f), FMath::Min(CurrentScale.Z + 0.01f, 3.6f));
			PortalMesh->SetRelativeScale3D(NewScale);

			// Call this function again after 0.1 seconds
			FTimerHandle TimerHandle;
			GetWorldTimerManager().SetTimer(TimerHandle, this, &APortal::ActivatePortal, 0.001f, false);
		}
		else {
			Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			ParticleSystem->Activate();
		}
	}

