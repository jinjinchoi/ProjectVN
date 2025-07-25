// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VNVehicleDestinationVolume.generated.h"

class UVNObjectPoolManager;
class UBoxComponent;

UCLASS()
class VNMINIGAME_API AVNVehicleDestinationVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	AVNVehicleDestinationVolume();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> BoxCollision;

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY()
	TObjectPtr<UVNObjectPoolManager> PoolManager;

};
