// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Core/GameMode/MiniGame/VNEnemyDefeatGameMode.h"

#include "DebugHelper.h"
#include "Core/GameInstance/VNGameInstance.h"

void AVNEnemyDefeatGameMode::RegisterSpawnVolume(AVNEnemySpawnVolume* SpawnVolume)
{
	SpawnVolumes.Add(SpawnVolume);
}

void AVNEnemyDefeatGameMode::SpawnVolumeCleared(AVNEnemySpawnVolume* SpawnVolume)
{
	SpawnVolumes.Remove(SpawnVolume);

	if (SpawnVolumes.Num() == 0)
	{
		DebugHelper::Print(TEXT("All Spawn Volume Cleared"), FColor::Green);
		PlayerWin();
	}
}

EMiniGameClearType AVNEnemyDefeatGameMode::GetMiniGameType() const
{
	return EMiniGameClearType::EnemyDefeat;
}

void AVNEnemyDefeatGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	UVNGameInstance* GameInstance = Cast<UVNGameInstance>(GetGameInstance());
	if (!GameInstance) return;

	// 풀 매니저 생성.
	GameInstance->SetEnemyDefeatPoolManager();
	// 풀 Empty 작업은 위젯 컨트롤러에서 직접 게임인스턴스에 접근하여 하고 있으며, 풀을 생성하는 부분과 비우는 부분이 달라 이를 수정해야할 필요가 있음.
	
}
