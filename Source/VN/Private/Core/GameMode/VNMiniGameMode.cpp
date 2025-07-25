// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Core/GameMode/VNMiniGameMode.h"
#include "DebugHelper.h"
#include "Core/GameInstance/VNGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Player/VNPlayerController.h"


void AVNMiniGameMode::PlayerLose()
{
	if (UVNGameInstance* GameInstance = Cast<UVNGameInstance>(GetGameInstance()))
	{
		GameInstance->bIsMiniGameCleared = false;
	}
	
	if (AVNPlayerController* PlayerController = Cast<AVNPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		PlayerController->CreateLoseScreen();
	}
	
}

EMiniGameClearType AVNMiniGameMode::GetMiniGameType() const
{
	return EMiniGameClearType::None;
}

void AVNMiniGameMode::BeginPlay()
{
	Super::BeginPlay();
	
}

void AVNMiniGameMode::PlayerWin() const
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
