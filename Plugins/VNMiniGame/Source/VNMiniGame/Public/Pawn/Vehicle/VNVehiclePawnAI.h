// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "Interface/PoolableActorInterface.h"
#include "Interface/VNPoolProviderInterface.h"
#include "SharedTypes/Public/Interface/VNVehicleInterface.h"
#include "VNVehiclePawnAI.generated.h"

class AAIController;
class UBoxComponent;
class UChaosWheeledVehicleMovementComponent;

/**
 * 
 */
UCLASS()
class VNMINIGAME_API AVNVehiclePawnAI : public AWheeledVehiclePawn, public IVNVehicleInterface, public IPoolableActorInterface
{
	GENERATED_BODY()

public:
	AVNVehiclePawnAI();
	
	/* begin IVNVehicleInterface */
	virtual void SetThrottle(const float InThrottle) override;
	virtual void SetBrake(const float InBrake) override;
	virtual void SetSteering(const float InSteering) override;
	virtual void SetRoad(const TArray<AActor*>& InRoad) override;
	virtual void SetRoadIndex(const int32 InRoadIndex) override;
	virtual AActor* GetCurrentRoad() const override;
	virtual TArray<AActor*> GetAllRoad() const override;
	virtual int32 GetCurrentLane() const override;
	virtual FVector GetFrontLocationOfCar() const override;
	virtual float GetCurrentSpeed() const override;
	virtual FVector GetForwardVector() const override;
	virtual void VehicleSpawn(const FTransform& TransformToSpawn) override;
	/* end IVNVehicleInterface */

	/* begin IPoolableActorInterface */
	virtual void OnActivateFromPool_Implementation() override;
	virtual void OnDeactivateToPool_Implementation() override;
	/* end IPoolableActorInterface */


protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> FrontSceneComponent;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> BoxCollision;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAudioComponent> AudioComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<USoundBase> CrashSound;
	
	UPROPERTY(EditAnywhere, Category = "Vehicle")
	TArray<AActor*> Road;

	/* 차량이 달리고 있는 로드의 인덱스. 스포너에서 스폰될때 랜덤으로 지정됨.  */
	int32 RoadIndex = 0;

	UPROPERTY(EditAnywhere, Category = "Vehicle")
	float LifeTime = 30.f;
	
private:
	UPROPERTY()
	TSet<AActor*> RecentlyHitActors;

	UPROPERTY()
	TObjectPtr<AController> CachedController;

	FTimerHandle LifeTimeHandle;
	
};
