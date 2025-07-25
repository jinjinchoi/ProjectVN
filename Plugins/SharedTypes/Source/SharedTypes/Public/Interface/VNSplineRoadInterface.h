// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VNSplineRoadInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UVNSplineRoadInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SHAREDTYPES_API IVNSplineRoadInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual FVector GetClosestLocationToRoad(const FVector& VehicleLocation, const float AdditionalDistance ) const = 0;
	virtual FVector GetSplineDirection(const FVector& TargetLocation) const = 0;
};
