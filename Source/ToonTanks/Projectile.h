// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class TOONTANKS_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	UStaticMeshComponent* mesh;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	class UProjectileMovementComponent* movementComponent;

	UFUNCTION(BlueprintCallable)
	void OnHit(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, FVector normalImpulse, const FHitResult& hit);

	UPROPERTY(EditAnywhere)
	float damageAmount = 50;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UParticleSystem* hitParticles; // not a component

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	class UParticleSystemComponent* trailParticles;

	UPROPERTY(EditAnywhere, Category = "Combat")
	class USoundBase* lauchSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* hitSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf <UCameraShakeBase> hitCameraShakeClass;

};
