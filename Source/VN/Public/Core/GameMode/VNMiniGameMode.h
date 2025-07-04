// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/GameMode/VNGameModeBase.h"
#include "Interface/VNEnemyDefeatGameModeInterface.h"
#include "VNMiniGameMode.generated.h"

UENUM()
enum class EMiniGameClearType
{
	None,
	EnemyDefeat,
	DistanceReached
};

/**
 * 
 */
UCLASS()
class VN_API AVNMiniGameMode : public AVNGameModeBase, public IVNEnemyDefeatGameModeInterface
{
	GENERATED_BODY()

public:
	virtual void RegisterSpawnVolume(AVNEnemySpawnVolume* SpawnVolume) override;
	virtual void SpawnVolumeCleared(AVNEnemySpawnVolume* SpawnVolume) override;
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Clear Info")
	EMiniGameClearType ClearType = EMiniGameClearType::None;

	/* 미니 게임 호감도 보상 */
	UPROPERTY(EditDefaultsOnly, Category = "Clear Info")
	TMap<FName /* 관련된 Character ID */, int32 /* 호감도 보상 */> AffectionRewardMap;

private:
	UPROPERTY()
	TSet<AVNEnemySpawnVolume*> SpawnVolumes;

	void HandleMiniGameClear();
	
};
