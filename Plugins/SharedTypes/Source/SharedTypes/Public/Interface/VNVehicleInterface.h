// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VNVehicleInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UVNVehicleInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SHAREDTYPES_API IVNVehicleInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetThrottle(const float InThrottle) = 0;
	virtual void SetBrake(const float InBrake) = 0;
	virtual void SetSteering(const float InSteering) = 0;

	virtual void SetRoad(const TArray<AActor*>& InRoad) = 0;
	virtual void SetRoadIndex(const int32 InRoadIndex) = 0;

	/* 현재 로드(스플라인 액터)를 얻는 함수 */
	virtual AActor* GetCurrentRoad() const = 0;
	/* 가지고 있는 모든 로드(스플라인 액터)를 가진 배열을 얻는 함수 */
	virtual TArray<AActor*> GetAllRoad() const = 0;
	/* 현재 달리고 있는 로드 배열의 인덱스를 얻는 함수 */
	virtual int32 GetCurrentLane() const = 0;
	/* 차량의 앞에 있는 신 컴포넌트의 위치를 구하는 함수 */
	virtual FVector GetFrontLocationOfCar() const = 0;
	virtual float GetCurrentSpeed() const = 0;
	virtual FVector GetForwardVector() const = 0;
	

	/* 차량이 스폰(Pool에서 소환)되고 실행시키는 함수 */
	virtual void VehicleSpawn(const FTransform& TransformToSpawn) = 0;
	
	
	
};
