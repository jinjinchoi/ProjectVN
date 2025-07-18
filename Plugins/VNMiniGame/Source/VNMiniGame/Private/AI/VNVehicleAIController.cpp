// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "AI/VNVehicleAIController.h"

#include "DebugHelper.h"
#include "Interface/VNSplineRoadInterface.h"
#include "Interface/VNVehicleInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


void AVNVehicleAIController::BeginPlay()
{
	Super::BeginPlay();

	CachedMaxSpeed = MaxSpeed;
	
}

void AVNVehicleAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	CachedVehicle = InPawn;
	if (CachedVehicle)
	{
		CachedSplineRoad = CachedVehicle->GetCurrentRoad();
	}

	SetActorTickEnabled(true);
	GetWorldTimerManager().SetTimer(OvertakingTimer, this, &ThisClass::TryOvertake, 3.f, true);
	
	
}

void AVNVehicleAIController::OnUnPossess()
{
	Super::OnUnPossess();

	SetActorTickEnabled(false);
	GetWorldTimerManager().ClearAllTimersForObject(this);
}

void AVNVehicleAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CachedVehicle) return;
	
	CachedVehicle->SetSteering(CalculateSteering());
	UpdateAccelerationState();

	if (IsVehicleFlipped())
	{
		TimeSinceFlipped += DeltaTime;
		if (TimeSinceFlipped > 2.0f && !bIsCurrentlyFlipped)
		{
			bIsCurrentlyFlipped = true;
			FlipUpright();
		}
	}
	else
	{
		TimeSinceFlipped = 0.0f;
		bIsCurrentlyFlipped = false;
	}
	
}

float AVNVehicleAIController::CalculateSteering() const
{
	if (!CachedSplineRoad)
	{
		const FString Msg = FString::Printf(TEXT("CurrentRoad is Invalid in %hs "), __FUNCTION__);
		DebugHelper::Print(Msg, FColor::Yellow);
		return 0;
	}
	
	const float AdditionalDistance = UKismetMathLibrary::MapRangeClamped(
		CachedVehicle->GetCurrentSpeed(), MinSpeed, MaxSpeed, 1000.f, 3000.f);
	
	const FVector ClosestLocationToSpline = CachedSplineRoad->GetClosestLocationToRoad(CachedVehicle->GetFrontLocationOfCar(), AdditionalDistance);
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(CachedVehicle->GetFrontLocationOfCar(), ClosestLocationToSpline);

	DrawDebugSphere(GetWorld(), ClosestLocationToSpline, 50.f, 12.f, FColor::Cyan);
	

	// 현재 Pawn 회전과 목표 회전 간 차이 계산
	// 지금 어디서 어디까지 얼마나 회전해야 하는지를 알아냄
	const FRotator DeltaRotation = (LookAtRotation - GetPawn()->GetActorRotation()).GetNormalized();

	const float LookYawValue = DeltaRotation.Yaw;
	

	// 현재 차량 위치와 도로 위치의 각도를 구하여 일정 각도 이상이면 회전 값을 전달.
	// 각도(InRange)를 너무 크게 잡으면 도로가 거의 꺽이지 않을 때 차량이 마찬가지로 거의 회전을 하지 않아 문제가 생길 수 있어 작은 각도부터 회전.
	return UKismetMathLibrary::MapRangeClamped(LookYawValue, -10.f, 10.f, -1.f, 1.f);
	
	
}

float AVNVehicleAIController::CalculateMaxSpeed() const
{
	if (!CachedSplineRoad)
	{
		const FString Msg = FString::Printf(TEXT("CurrentRoad is Invalid in %hs "), __FUNCTION__);
		DebugHelper::Print(Msg, FColor::Yellow);
		return 0;
	}

	const float AdditionalDistance = UKismetMathLibrary::MapRangeClamped(
		CachedVehicle->GetCurrentSpeed(), MinSpeed, MaxSpeed, 3000.f, 9000.f);
	
	
	const FVector ClosestLocationToSpline = CachedSplineRoad->GetClosestLocationToRoad(CachedVehicle->GetFrontLocationOfCar(), AdditionalDistance);

	DrawDebugSphere(GetWorld(), ClosestLocationToSpline, 50.f, 12.f, FColor::Green);
	
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(CachedVehicle->GetFrontLocationOfCar(), ClosestLocationToSpline);
	const FRotator DeltaRotation = (LookAtRotation - GetPawn()->GetActorRotation()).GetNormalized();

	const float LookYawValue = DeltaRotation.Yaw;
	const float AbsYawValue = FMath::Abs(LookYawValue);
	
	// 차량과 길의 각도를 계산하여 각도가 높아질수록 낮은 최대 속도를 반환
	return UKismetMathLibrary::MapRangeClamped(AbsYawValue, 0, 40, MaxSpeed, MinSpeed);
	
}

void AVNVehicleAIController::UpdateAccelerationState() const
{
	if (!CachedVehicle) return;
	
	const float CurrentSpeed = CachedVehicle->GetCurrentSpeed();
	const float TopSpeed = CalculateMaxSpeed();
	const float SpeedDiff = TopSpeed - CurrentSpeed;
	
	if (SpeedDiff > SpeedTolerance)
	{
		const float ThrottleValue = FMath::Clamp(SpeedDiff / TopSpeed, 0.f, 1.f);
		
		CachedVehicle->SetThrottle(ThrottleValue);
		CachedVehicle->SetBrake(0.f);
	}
	else if (SpeedDiff < -SpeedTolerance)
	{
		const float BrakeValue = FMath::Clamp(-SpeedDiff / MaxSpeed, 0.f, 1.f);
		
		CachedVehicle->SetBrake(BrakeValue);
		CachedVehicle->SetThrottle(0.f);
	}
	else
	{
		CachedVehicle->SetThrottle(0.f);
		CachedVehicle->SetBrake(0.f);
	}
	
}

FHitResult AVNVehicleAIController::IsVehicleAhead(const FVector& Start, const FVector& Forward)
{
	const FVector BoxHalfExtent = FVector(100.f, 100.f, 50.f); 
	const FQuat Rotation = FQuat::Identity;
	const FVector End = Start + Forward * CheckAheadDistance;

	FHitResult HitResult;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Vehicle);
	
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	CollisionQueryParams.AddIgnoredActor(GetPawn());

	bool bHit = GetWorld()->SweepSingleByObjectType(
		HitResult,
		Start,
		End,
		Rotation,
		ObjectQueryParams,
		FCollisionShape::MakeBox(BoxHalfExtent),
		CollisionQueryParams
	);
	
#if WITH_EDITOR
	// 디버그 박스 트레이스
	if (bShowDebug)
	{
		FColor LineColor = bHit ? FColor::Red : FColor::Green;
		DrawDebugBox(GetWorld(), Start, BoxHalfExtent, Rotation, LineColor, false, 1.0f);
		DrawDebugBox(GetWorld(), End, BoxHalfExtent, Rotation, LineColor, false, 1.0f);
		DrawDebugLine(GetWorld(), Start, End, LineColor, false, 1.0f, 0, 1.f);
	}
#endif

	return HitResult;
	
}

TArray<int32> AVNVehicleAIController::GetAdjacentLanes(const int32 CurrentIndex, const int32 MaxIndex)
{
	TArray<int32> AdjacentLanes;

	if (CurrentIndex > 0)
	{
		AdjacentLanes.Add(CurrentIndex - 1);  // 왼쪽 차선
	}

	if (CurrentIndex < MaxIndex)
	{
		AdjacentLanes.Add(CurrentIndex + 1);  // 오른쪽 차선
	}

	return AdjacentLanes;
}

int32 AVNVehicleAIController::GetAvailableLaneIndex()
{
	TArray<AActor*> EntireRoad = CachedVehicle->GetAllRoad();
	const int32 NumRoad = EntireRoad.Num();
	if (NumRoad <= 1) return -1;
	
	const FVector CarFrontLocation = CachedVehicle->GetFrontLocationOfCar();
	const FVector CarForward = CachedVehicle->GetForwardVector();

	TArray<int32> AllowedLanes = GetAdjacentLanes(CachedVehicle->GetCurrentLane(), NumRoad);

	for (const int32 Index: AllowedLanes)
	{
		if (Index < 0 || Index >= NumRoad) continue;
		
		const IVNSplineRoadInterface* AdjacentLand = Cast<IVNSplineRoadInterface>(EntireRoad[Index]);
		if (!AdjacentLand) continue;
		
		const FVector NewLaneLocation = AdjacentLand->GetClosestLocationToRoad(CarFrontLocation, 0.f);

		const FHitResult HitResult = IsVehicleAhead(NewLaneLocation, CarForward);
		
		if (!HitResult.bBlockingHit)
		{
			return Index;
		}
		
	}

	return -1;
	
}


void AVNVehicleAIController::TryOvertake()
{
	if (!CachedVehicle || !CachedSplineRoad) return;

	FVector VehicleFrontLocation = CachedVehicle->GetFrontLocationOfCar();
	FVector ClosestSplineLocation = CachedSplineRoad->GetClosestLocationToRoad(VehicleFrontLocation, 0.f);
	VehicleFrontLocation += FVector(0.f, 0.f, 50.f);

	FHitResult CurrentLaneHitResult = IsVehicleAhead(VehicleFrontLocation, CachedSplineRoad->GetSplineDirection(ClosestSplineLocation));
	if (!CurrentLaneHitResult.bBlockingHit)
	{
		MaxSpeed = CachedMaxSpeed;
		return;
	}

#if WITH_EDITOR
	if (bShowDebug)
	{
		FString Msg = FString::Printf(TEXT("Hit Actor: %s (%s)"),
			*CurrentLaneHitResult.GetActor()->GetName(), *CurrentLaneHitResult.GetActor()->GetClass()->GetName());
		DebugHelper::Print(Msg);
	}
#endif

	// 앞차와 본인 차의 속도를 비교하여 앞차가 더 빠르면 추월 시도 안함
	const IVNVehicleInterface* FrontVehicle = Cast<IVNVehicleInterface>(CurrentLaneHitResult.GetActor());
	if (!FrontVehicle || CachedVehicle->GetCurrentSpeed() <= FrontVehicle->GetCurrentSpeed())
	{
		return;
	}
	
	
	// 추월이 불가능하면 앞의 차량에 맞게 속도에 맞춰 속도 하향.
	// 추월이 가능하면 레인 옮기기.

	const int32 LaneIndex = GetAvailableLaneIndex();
	
	if (LaneIndex >= 0)
	{
		CachedVehicle->SetRoadIndex(LaneIndex);
		CachedSplineRoad = CachedVehicle->GetCurrentRoad();
	}
	else
	{
		MaxSpeed = FrontVehicle->GetCurrentSpeed();
	}
	
}

bool AVNVehicleAIController::IsVehicleFlipped() const
{
	if (!GetPawn()) return false;
	
	const FVector PawnUpVector = GetPawn()->GetActorUpVector();
	const float Dot = FVector::DotProduct(PawnUpVector, FVector::UpVector);

	const FVector Velocity = GetPawn()->GetVelocity();
	const float Speed = Velocity.Size();

	constexpr float TiltThreshold = 0.5f; 
	constexpr float SpeedThreshold = 10.0f;

	const bool bTiltedTooMuch = Dot < TiltThreshold;
	const bool bNotMoving = Speed < SpeedThreshold;
	
	return bTiltedTooMuch && bNotMoving;
	
}

void AVNVehicleAIController::FlipUpright() const
{
	FVector Location = GetPawn()->GetActorLocation();
	FRotator UprightRotation = FRotator(0.f, GetPawn()->GetActorRotation().Yaw, 0.f);
	Location.Z += 100.f;
	
	GetPawn()->SetActorLocationAndRotation(Location, UprightRotation, false, nullptr, ETeleportType::TeleportPhysics);

	DebugHelper::Print(TEXT("차량 복구작업진행"));
	
}
