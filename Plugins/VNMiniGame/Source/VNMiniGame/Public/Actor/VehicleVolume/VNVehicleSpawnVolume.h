// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VNVehicleSpawnVolume.generated.h"

class UBoxComponent;
class UVNObjectPoolManager;
class AVNVehiclePawnAI;

USTRUCT(BlueprintType)
struct FSpawnTimeParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Spawn")
	int32 SpawnLevel = 1;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	float SpawnTime = 2.5f;
	
};

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
	TArray<FSpawnTimeParams> SpawnTimeParams;

	UPROPERTY(EditAnywhere, Category = "Spawn Properties",  meta = (MustImplement = "VNSplineRoadInterface"))
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
