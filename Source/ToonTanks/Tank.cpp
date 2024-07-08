// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
//#include "DrawDebugHelpers.h"

ATank::ATank()
{
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	springArm->SetupAttachment(RootComponent);

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(springArm);
}

void ATank::BeginPlay()
{
	Super::BeginPlay();

	playerController = Cast<APlayerController>(GetController());
}

void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (playerController)
	{
		FHitResult hitResult;
		playerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, hitResult); // block the visibility
		RotateTurret(hitResult.ImpactPoint);
	}
}

void ATank::Move(float value)
{
	FVector deltaLocation = FVector::ZeroVector;
	float deltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	deltaLocation.X = value * deltaTime * speed;
	AddActorLocalOffset(deltaLocation, true); // change actor location in local space; if bsweep = true, detect collision to avoid overlap
}

void ATank::Turn(float value)
{
	FRotator deltaRotation = FRotator::ZeroRotator;
	float deltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	deltaRotation.Yaw = value * deltaTime * turnRate;
	AddActorLocalRotation(deltaRotation, true);
}

void ATank::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move); // link the input with a function in class
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();
	SetActorHiddenInGame(true); // hide the tank from screen
	SetActorTickEnabled(false); // turn off the tick

	bAlive = false;
}

APlayerController* ATank::GetPlayerController() const
{
	return playerController;
}