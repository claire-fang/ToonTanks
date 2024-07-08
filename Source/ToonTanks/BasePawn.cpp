// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = capsule;

	baseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	baseMesh->SetupAttachment(capsule); // attach the base mesh to the capsule; a func inherited from scene component

	turretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	turretMesh->SetupAttachment(baseMesh);

	projectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	projectileSpawnPoint->SetupAttachment(turretMesh);
}

// Called when the game starts or when spawned
void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABasePawn::RotateTurret(FVector target)
{
	FVector toTarget = target - turretMesh->GetComponentLocation();
	FRotator lookAtRotation = FRotator(0, toTarget.Rotation().Yaw, 0);

	turretMesh->SetWorldRotation(lookAtRotation);
}

void ABasePawn::Fire()
{
	auto projectile = GetWorld()->SpawnActor<AProjectile>(projectileClass, projectileSpawnPoint->GetComponentLocation(), projectileSpawnPoint->GetComponentRotation());
	projectile->SetOwner(this);
}

void ABasePawn::HandleDestruction() 
{
	if (hitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, hitParticles, GetActorLocation(), GetActorRotation());
	}

	if (deathSound) 
	{
		UGameplayStatics::PlaySoundAtLocation(this, deathSound, GetActorLocation());
	}

	if (deathCameraShakeClass)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(deathCameraShakeClass);
	}
}

