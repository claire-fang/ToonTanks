// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h"
#include "ToonTanksPlayerController.h"

void AToonTanksGameMode::BeginPlay()
{
	Super::BeginPlay();

	HandleGameStart();

	StartGame();
}

void AToonTanksGameMode::ActorDied(AActor* deadActor)
{
	if (deadActor == tank)
	{
		tank->HandleDestruction();
		if (playerController)
		{
			playerController->SetPlayerEnabledState(false);
		}
		GameOver(false);
	}
	else if (ATower* destroyedTower = Cast<ATower>(deadActor))
	{
		destroyedTower->HandleDestruction();
		targetTowers -= 1;
		if (targetTowers == 0)
		{
			GameOver(true);
		}
	}
}

void AToonTanksGameMode::HandleGameStart()
{
	targetTowers = GetTargetTowerCount();
	tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	playerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	StartGame();

	if (playerController)
	{
		playerController->SetPlayerEnabledState(false);

		FTimerHandle playerEnableTimerHandle;
		FTimerDelegate playerEnableTimerDelegate = FTimerDelegate::CreateUObject(playerController, &AToonTanksPlayerController::SetPlayerEnabledState, true);
		GetWorldTimerManager().SetTimer(playerEnableTimerHandle, playerEnableTimerDelegate, startDelay, false);
	}


}

int32 AToonTanksGameMode::GetTargetTowerCount()
{
	TArray<AActor*> towers;
	UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), towers);
	return towers.Num();
}