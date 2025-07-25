// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Actor/Path/VNRoadSpline.h"

#include "Components/SplineComponent.h"

AVNRoadSpline::AVNRoadSpline()
{
	PrimaryActorTick.bCanEverTick = false;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	SplineComponent->SetupAttachment(RootComponent);

}

FVector AVNRoadSpline::GetClosestLocationToRoad(const FVector& VehicleLocation, const float AdditionalDistance) const
{
	float DistanceToAlongSpline = SplineComponent->GetDistanceAlongSplineAtLocation(VehicleLocation, ESplineCoordinateSpace::World);
	DistanceToAlongSpline += AdditionalDistance;

	const float SplineLength = SplineComponent->GetSplineLength();
	
	if (SplineLength < DistanceToAlongSpline)
	{
		const float AdjustedDistance = DistanceToAlongSpline - SplineLength;
		return SplineComponent->GetWorldLocationAtDistanceAlongSpline(AdjustedDistance);
	}

	return SplineComponent->GetWorldLocationAtDistanceAlongSpline(DistanceToAlongSpline);
}

FVector AVNRoadSpline::GetSplineDirection(const FVector& TargetLocation) const
{
	const float DistanceToAlongSpline = SplineComponent->GetDistanceAlongSplineAtLocation(TargetLocation, ESplineCoordinateSpace::World);
	return SplineComponent->GetDirectionAtDistanceAlongSpline(DistanceToAlongSpline, ESplineCoordinateSpace::World);
}


// Called when the game starts or when spawned
void AVNRoadSpline::BeginPlay()
{
	Super::BeginPlay();
	
}
