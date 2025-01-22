// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/HitInterface.h"  
#include "Macros.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    RootComponent = SphereComponent;
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->InitialSpeed = 1000.f;
    ProjectileMovementComponent->MaxSpeed = 3000.f;
    ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->bShouldBounce = false; 
    SphereComponent->InitSphereRadius(5.f); 
    SphereComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
}

void AProjectile::BeginPlay()
{
    Super::BeginPlay();
    SphereComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
    Tags.Add("Projectile");
}

void AProjectile::SetTarget(AActor* ProjectileTarget)
{
    if (ProjectileMovementComponent && ProjectileTarget)
    {
        FVector TargetDirection = (ProjectileTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        SphereComponent->IgnoreActorWhenMoving(GetOwner(), true);
        ProjectileMovementComponent->Velocity = TargetDirection * ProjectileMovementComponent->InitialSpeed;
    }
}

//void AProjectile::Reflect(APawn* Reflector, AActor* Weapon)
//{
//    if (!Reflector||!SphereComponent) return;
//    bReflected = true;
//    APawn* TargetedActor = GetInstigator();
//        SphereComponent->IgnoreActorWhenMoving(Weapon,true);
//    SphereComponent->IgnoreActorWhenMoving(TargetedActor, false);
//    TargetFaction = FName("EnemyFaction");
//    SetOwner(Reflector);
//    SetInstigator(Reflector);
//    SetTarget(TargetedActor);
//}



void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!OtherActor) return;
    if (HitSound)
        UGameplayStatics::PlaySoundAtLocation(
            this,
            HitSound,
            GetActorLocation());
    if (HitSparks) {
        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            HitSparks,
            FTransform(GetActorRotation(), GetActorLocation(), FVector(.5f))
        );
    }
    if (!OtherActor->ActorHasTag(TargetFaction)) {
        Destroy();
        return;
    }

    IHitInterface* HitCreature = Cast<IHitInterface>(OtherActor);
    if (HitCreature) {
        UGameplayStatics::ApplyDamage(
            Hit.GetActor(),
            DamageValue,
            GetInstigator()->GetController(),
            this,
            UDamageType::StaticClass());
        CreateFields(GetActorLocation());
        HitCreature->Execute_GetHit(Hit.GetActor(), Hit.ImpactPoint, Hit.ImpactNormal, GetOwner()->GetActorLocation());
       // if(!bReflected)
        Destroy();
       // bReflected = false;
    }
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

