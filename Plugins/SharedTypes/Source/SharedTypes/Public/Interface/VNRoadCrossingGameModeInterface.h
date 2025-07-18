// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VNRoadCrossingGameModeInterface.generated.h"

class AVNItemSpawner;

DECLARE_MULTICAST_DELEGATE(FOnPoolManagerSet);
DECLARE_MULTICAST_DELEGATE(FOnLevelStarted);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UVNRoadCrossingGameModeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SHAREDTYPES_API IVNRoadCrossingGameModeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual FOnPoolManagerSet& GetPoolManagerSetDelegate() = 0;
	virtual FOnLevelStarted& GetOnLevelStartedDelegate() = 0;
	virtual bool IsBroadcasted() const = 0;
	virtual int32 GetCurrentMapLevel() const = 0;
	virtual void RegisterItemSpawner(AVNItemSpawner* InItemSpawner) = 0;
	virtual void AllItemPickedUp() = 0;
	
};
