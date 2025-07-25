// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/VNSplineRoadInterface.h"
#include "VNRoadSpline.generated.h"

class USplineComponent;

UCLASS()
class VNMINIGAME_API AVNRoadSpline : public AActor, public IVNSplineRoadInterface
{
	GENERATED_BODY()
	
public:	
	AVNRoadSpline();

	/* begin IVNSplineRoadInterface */
	virtual FVector GetClosestLocationToRoad(const FVector& VehicleLocation, const float AdditionalDistance) const override;
	virtual FVector GetSplineDirection(const FVector& TargetLocation) const override;
	/* end IVNSplineRoadInterface */
	

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> SplineComponent;



};
