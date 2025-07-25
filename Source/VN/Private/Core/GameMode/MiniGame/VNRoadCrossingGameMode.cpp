// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Core/GameMode/MiniGame/VNRoadCrossingGameMode.h"

#include "DebugHelper.h"
#include "Actor/Item/VNItemSpawner.h"
#include "Core/GameInstance/VNGameInstance.h"
#include "Kismet/GameplayStatics.h"


EMiniGameClearType AVNRoadCrossingGameMode::GetMiniGameType() const
{
	return EMiniGameClearType::LoadCrossing;
}

FOnPoolManagerSet& AVNRoadCrossingGameMode::GetPoolManagerSetDelegate()
{
	return PoolManagerSetDelegate;
}

FOnLevelStarted& AVNRoadCrossingGameMode::GetOnLevelStartedDelegate()
{
	return OnLevelStartedDelegate;
}

bool AVNRoadCrossingGameMode::IsBroadcasted() const
{
	return bIsBroadcasted;
}


int32 AVNRoadCrossingGameMode::GetCurrentMapLevel() const
{
	return CurrentLevel;
}

void AVNRoadCrossingGameMode::RegisterItemSpawner(AVNItemSpawner* InItemSpawner)
{
	if (!ItemSpawner)
	{
		ItemSpawner = InItemSpawner;
		ItemSpawner->SpawnItem();
	}
}


void AVNRoadCrossingGameMode::BeginPlay()
{
	Super::BeginPlay();

	PickedUpItemCount = 0;

	UVNGameInstance* GameInstance = Cast<UVNGameInstance>(GetGameInstance());
	if (!GameInstance) return;

	GameInstance->SetCrossingPoolManager();
	
	PoolManagerSetDelegate.Broadcast();
	OnLevelStartedDelegate.Broadcast();
	
}

void AVNRoadCrossingGameMode::AllItemPickedUp()
{
	if (CurrentLevel < ClearLevel)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
		if (PC)
		{
			RestartPlayer(PC);
			CurrentLevel++;
			OnLevelStartedDelegate.Broadcast();
			ItemSpawner->SpawnItem();
		}
	}
	else
	{
		PlayerWin();
	}
	
}
