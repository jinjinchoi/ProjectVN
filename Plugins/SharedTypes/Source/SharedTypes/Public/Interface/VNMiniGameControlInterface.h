// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VNMiniGameControlInterface.generated.h"

enum class EMiniGameClearType : uint8
{
	None,
	EnemyDefeat,
	DistanceReached,
	LoadCrossing
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UVNMiniGameControlInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SHAREDTYPES_API IVNMiniGameControlInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void PlayerLose() = 0;
	virtual EMiniGameClearType GetMiniGameType() const = 0;
	
};
