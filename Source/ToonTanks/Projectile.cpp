// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = mesh;

	movementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	movementComponent->MaxSpeed = 1300;
	movementComponent->InitialSpeed = 1000;

	trailParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke Trail"));
	trailParticles->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	mesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit); // bound onHit to onComponentHit, which is a hit event aka a multicast delegate; when a hit event happens, all bounded functions will be called

	if (lauchSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, lauchSound, GetActorLocation());
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, FVector normalImpulse, const FHitResult& hit)
{
	auto owner = GetOwner();
	if (!owner)
	{
		Destroy();
		return;
	}
	if (otherActor && otherActor != this && otherActor != owner)
	{
		auto damageTypeClass = UDamageType::StaticClass();
		UGameplayStatics::ApplyDamage(otherActor, damageAmount, GetOwner()->GetInstigatorController(), this, damageTypeClass); // generate a damage event; the OnTakeAnyDamage will broadcast and respond to it

		if (hitParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, hitParticles, GetActorLocation(), GetActorRotation());
		}

		if (hitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, hitSound, GetActorLocation());
		}

		if (hitCameraShakeClass)
		{
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(hitCameraShakeClass);
		}
	}
	Destroy();
}

