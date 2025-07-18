// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "VNVehicleAIController.generated.h"

class IVNVehicleInterface;
class IVNSplineRoadInterface;
/**
 * 
 */
UCLASS()
class VNMINIGAME_API AVNVehicleAIController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, Category="User Params")
	float MinSpeed = 40.f;
	
	UPROPERTY(EditAnywhere, Category="User Params")
	float MaxSpeed = 150.f;

	UPROPERTY(EditAnywhere, Category="User Params")
	float SpeedTolerance = 10.f;

	/* 앞에 차가 존재하는지 확인하는 거리 */
	UPROPERTY(EditAnywhere, Category="User Params")
	float CheckAheadDistance = 1000.f;

	/* 추월 관련 로직에서 디버그 메세지랑 박스 보일지 여부 */
	UPROPERTY(EditAnywhere, Category="User Params")
	bool bShowDebug = false;

private:
	
#pragma region MoveVehicle // 차량 움직임 로직
	float CalculateSteering() const;
	float CalculateMaxSpeed() const;
	void UpdateAccelerationState() const;
#pragma endregion

#pragma region Overtaking // 차량 추월 로직
	/* 차량이 앞에 있는지 확인 */
	FHitResult IsVehicleAhead(const FVector& Start, const FVector& Forward);
	/* 현제 레인에 따라 이동할 수 있는 레인 번호 반환 */
	TArray<int32> GetAdjacentLanes(const int32 CurrentIndex, const int32 MaxIndex);
	/* 반환된 레인 번호에서 이동 가능한 차선을 알아내는 함수. 마이너스 값은 이동할 수 있는 차선 없음 */
	int32 GetAvailableLaneIndex();
	void TryOvertake();
	
	FTimerHandle OvertakingTimer;
	float CachedMaxSpeed = 150.f;
	
#pragma endregion

	bool IsVehicleFlipped() const;
	void FlipUpright() const;
	float TimeSinceFlipped = 0.0f;
	bool bIsCurrentlyFlipped = false;
	
	TScriptInterface<IVNSplineRoadInterface> CachedSplineRoad;
	TScriptInterface<IVNVehicleInterface> CachedVehicle;

	
};
