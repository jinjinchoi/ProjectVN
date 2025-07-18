// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VNEnemyDefeatGameModeInterface.generated.h"

class AVNEnemySpawnVolume;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UVNEnemyDefeatGameModeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SHAREDTYPES_API IVNEnemyDefeatGameModeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void RegisterSpawnVolume(AVNEnemySpawnVolume* SpawnVolume) = 0;
	virtual void SpawnVolumeCleared(AVNEnemySpawnVolume* SpawnVolume) = 0;
	
};
