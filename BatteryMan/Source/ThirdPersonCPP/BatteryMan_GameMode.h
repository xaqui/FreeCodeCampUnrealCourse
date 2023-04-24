// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BatteryMan_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSONCPP_API ABatteryMan_GameMode : public AGameMode
{
	GENERATED_BODY()

	ABatteryMan_GameMode();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		TSubclassOf<APawn> RechargePickup;

	float SpawnZ = 500.0f;

	UPROPERTY(EditAnywhere)
		float SpawnXMin;
	UPROPERTY(EditAnywhere)
		float SpawnXMax;
	UPROPERTY(EditAnywhere)
		float SpawnYMin;
	UPROPERTY(EditAnywhere)
		float SpawnYMax;

	void SpawnRechargePickup();

};
