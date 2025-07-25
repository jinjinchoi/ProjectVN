// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/GameMode/VNMiniGameMode.h"
#include "Interface/VNRoadCrossingGameModeInterface.h"
#include "VNRoadCrossingGameMode.generated.h"

class AVNItemSpawner;

/**
 * 
 */
UCLASS()
class VN_API AVNRoadCrossingGameMode : public AVNMiniGameMode, public IVNRoadCrossingGameModeInterface
{
	GENERATED_BODY()

public:
	/* begin IVNMiniGameControlInterfacer */
	virtual EMiniGameClearType GetMiniGameType() const override;
	/* end IVNMiniGameControlInterfacer */

	/* begin IVNRoadCrossingGameModeInterface */
	virtual FOnPoolManagerSet& GetPoolManagerSetDelegate() override;
	virtual FOnLevelStarted& GetOnLevelStartedDelegate() override;
	virtual bool IsBroadcasted() const override;
	virtual int32 GetCurrentMapLevel() const override;
	virtual void RegisterItemSpawner(AVNItemSpawner* InItemSpawner) override;
	virtual void AllItemPickedUp() override;
	/* end IVNRoadCrossingGameModeInterface */

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Game Properties")
	int32 ClearLevel = 1;
	

private:
	FOnPoolManagerSet PoolManagerSetDelegate;
	FOnLevelStarted OnLevelStartedDelegate;

	UPROPERTY()
	TObjectPtr<AVNItemSpawner> ItemSpawner;
	
	int32 PickedUpItemCount = 0;
	int32 CurrentLevel = 1;

	bool bIsBroadcasted = false;
	
};
