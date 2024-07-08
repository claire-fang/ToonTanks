// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (InFireRange())
	{
		RotateTurret(tank->GetActorLocation());
	}

}

void ATower::BeginPlay()
{
	Super::BeginPlay();

	tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

	GetWorldTimerManager().SetTimer(fireRateTimerHandle, this, &ATower::CheckFireCondition, fireRate, true); // called the function every 2 seconds in this context
}

void ATower::CheckFireCondition()
{
	if (!tank)
	{
		return;
	}

	if (InFireRange() && tank->bAlive)
	{
		// UE_LOG(LogTemp, Display, TEXT("Debug check fire condition"));
		Fire();
	}
}

bool ATower::InFireRange()
{
	if (tank)
	{
		float distance = FVector::Dist(tank->GetActorLocation(), GetActorLocation());
		if (distance <= fireRange)
		{
			return true;
		}
	}
	return false;
}

void ATower::HandleDestruction()
{
	Super::HandleDestruction();
	Destroy();
}