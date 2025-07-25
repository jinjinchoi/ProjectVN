// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/GameMode/VNMiniGameMode.h"
#include "Interface/VNEnemyDefeatGameModeInterface.h"
#include "VNEnemyDefeatGameMode.generated.h"

/**
 * 
 */
UCLASS()
class VN_API AVNEnemyDefeatGameMode : public AVNMiniGameMode, public IVNEnemyDefeatGameModeInterface
{
	GENERATED_BODY()

public:
	/* begin IVNEnemyDefeatGameModeInterface */
	virtual void RegisterSpawnVolume(AVNEnemySpawnVolume* SpawnVolume) override;
	virtual void SpawnVolumeCleared(AVNEnemySpawnVolume* SpawnVolume) override;
	/* end IVNEnemyDefeatGameModeInterface*/

	/* begin IVNMiniGameControlInterfacer */
	virtual EMiniGameClearType GetMiniGameType() const override;
	/* end IVNMiniGameControlInterfacer */

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TSet<AVNEnemySpawnVolume*> SpawnVolumes;
	
};
