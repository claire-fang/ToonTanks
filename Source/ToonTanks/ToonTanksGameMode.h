// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ToonTanksGameMode.generated.h"


UCLASS()
class TOONTANKS_API AToonTanksGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void ActorDied(AActor* deadActor);

protected:
	void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();

	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool bWonGame);

private:
	class ATank* tank;
	
	class AToonTanksPlayerController* playerController;

	float startDelay = 3; // wait 3s before we start the game

	int32 targetTowers;

	int32 GetTargetTowerCount();

	void HandleGameStart();
};
