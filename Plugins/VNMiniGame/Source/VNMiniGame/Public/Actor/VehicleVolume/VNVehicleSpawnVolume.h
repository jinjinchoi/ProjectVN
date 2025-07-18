// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VNVehicleSpawnVolume.generated.h"

class UBoxComponent;
class UVNObjectPoolManager;
class AVNVehiclePawnAI;


UCLASS()
class VNMINIGAME_API AVNVehicleSpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	AVNVehicleSpawnVolume();
	void ActivateSpawnVolume();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> BoxCollision;
	
	UPROPERTY(EditAnywhere, Category = "Spawn Properties", meta = (MustImplement = "VNVehicleInterface"))
	TArray<TSubclassOf<AActor>> VehicleClasses;

	UPROPERTY(EditAnywhere, Category = "Spawn Properties")
	TObjectPtr<UCurveFloat> SpawnDelayCurve;

	UPROPERTY(EditAnywhere, Category = "Spawn Properties")
	TArray<AActor*> Road;
	

private:
	void StartSpawnTimer();
	void SpawnVehicle();
	float GetSpawnTime() const;
	bool IsSpawnLocationOccupied() const;
	
	UPROPERTY()
	TObjectPtr<UVNObjectPoolManager> PoolManager;

	FTimerHandle SpawnTimer;
	float SpawnLevel = 1;
	

};
