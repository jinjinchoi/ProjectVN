// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Core/GameMode/VNMiniGameMode.h"

#include "DebugHelper.h"
#include "VNFunctionLibrary.h"
#include "Core/GameInstance/VNGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Player/VNPlayerController.h"
#include "UI/HUD/VNDialogueHUD.h"

void AVNMiniGameMode::RegisterSpawnVolume(AVNEnemySpawnVolume* SpawnVolume)
{
	if (ClearType != EMiniGameClearType::EnemyDefeat)
	{
		DebugHelper::Print(TEXT("you must set it to the correct EMiniGameClearType"), FColor::Yellow);
		return;
	}
	SpawnVolumes.Add(SpawnVolume);
}

void AVNMiniGameMode::SpawnVolumeCleared(AVNEnemySpawnVolume* SpawnVolume)
{
	if (ClearType != EMiniGameClearType::EnemyDefeat)
	{
		DebugHelper::Print(TEXT("you must set it to the correct EMiniGameClearType"), FColor::Yellow);
		return;
	}
	
	SpawnVolumes.Remove(SpawnVolume);

	if (SpawnVolumes.Num() == 0)
	{
		DebugHelper::Print(TEXT("All Spawn Volume Cleared"), FColor::Green);
		HandleMiniGameClear();
	}
	
	
}

void AVNMiniGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (ClearType == EMiniGameClearType::None)
	{
		DebugHelper::Print(TEXT("You need to set ClearType"), FColor::Yellow);
	}

	UVNGameInstance* GameInstance = Cast<UVNGameInstance>(GetGameInstance());
	if (!GameInstance) return;

	// 풀 매니저 생성.
	GameInstance->SetMiniGameSubSystem();
	// 풀 Empty 작업은 위젯 컨트롤러에서 직접 게임인스턴스에 접근하여 하고 있으며, 풀을 생성하는 부분과 비우는 부분이 달라 이를 수정해야할 필요가 있음.
	
}

void AVNMiniGameMode::HandleMiniGameClear()
{
	UVNGameInstance* GameInstance = Cast<UVNGameInstance>(GetGameInstance());
	AVNPlayerController* PlayerController = Cast<AVNPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (!GameInstance || !PlayerController)
	{
		DebugHelper::Print(TEXT("Player Controller or Game Instance is null"), FColor::Red);
		return;
	}
	
	GameInstance->bIsMiniGameCleared = true;
	GameInstance->AffectionRewardMap = AffectionRewardMap;
	
	PlayerController->CreateWinScreen();
	
	
}
