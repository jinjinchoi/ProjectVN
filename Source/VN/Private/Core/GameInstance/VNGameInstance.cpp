// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "VN/Public/Core/GameInstance/VNGameInstance.h"

#include "VNDialogueManager.h"
#include "Core/GameInstance/MiniGameGameInstanceSubsystem.h"
#include "Core/SaveGame/VNSaveGame.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/Map/VNLevelManager.h"
#include "Manager/Save/VNSaveManager.h"
#include "Manager/Sound/VNSoundManager.h"


void UVNGameInstance::Init()
{
	Super::Init();

	check(DialogueManagerClass && SoundManagerClass && LevelManagerClass)

	CreateManagerClasses();
	SetUserConfig();
}

void UVNGameInstance::ResetGameMode()
{
	CurrentSaveSlotName = FString();
	CurrentSaveSlotIndex = -1;
	bIsPlayingMiniGame = false;
	bIsMiniGameCleared = false;
	NextDialogueId = NAME_None;
	MiniGameNodeId = NAME_None;
	AffectionRewardMap.Empty();
}

void UVNGameInstance::ResetMiniGameVariables()
{
	bIsPlayingMiniGame = false;
	bIsMiniGameCleared = false;
	NextDialogueId = NAME_None;
	MiniGameNodeId = NAME_None;
	AffectionRewardMap.Empty();
}

void UVNGameInstance::OnStart()
{
	Super::OnStart();

	SetSoundMix();
	
}

void UVNGameInstance::CreateManagerClasses()
{
	SoundManager = NewObject<UVNSoundManager>(this, SoundManagerClass);
	
	DialogueManager = NewObject<UVNDialogueManager>(this, DialogueManagerClass);
	DialogueManager->Init();
	
	SaveManager = NewObject<UVNSaveManager>(this);
	SaveManager->Init();

	LevelManager = NewObject<UVNLevelManager>(this, LevelManagerClass);
	
}

void UVNGameInstance::SetUserConfig()
{
	if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
	{
		Settings->LoadSettings(); 
		Settings->ApplySettings(false);
	}
}


void UVNGameInstance::SetSoundMix() const
{
	check(SoundMix && MasterSoundClass && BGMSoundClass && SFXSoundClass && UISoundClass);

	const float LoadedMasterVolume = SaveManager->GetMetaSaveGame()->MasterVolume;
	const float LoadedBGMVolume = SaveManager->GetMetaSaveGame()->BGMVolume;
	const float LoadedSFXVolume = SaveManager->GetMetaSaveGame()->SFXVolume;
	const float LoadedUIVolume = SaveManager->GetMetaSaveGame()->UIVolume;
	
	UGameplayStatics::SetBaseSoundMix(this, SoundMix);
	UGameplayStatics::SetSoundMixClassOverride(this, SoundMix, MasterSoundClass, LoadedMasterVolume);
	UGameplayStatics::SetSoundMixClassOverride(this, SoundMix, BGMSoundClass, LoadedBGMVolume);
	UGameplayStatics::SetSoundMixClassOverride(this, SoundMix, SFXSoundClass, LoadedSFXVolume);
	UGameplayStatics::SetSoundMixClassOverride(this, SoundMix, UISoundClass, LoadedUIVolume);
	
	
}

void UVNGameInstance::SetMiniGameSubSystem() const
{
	if (UMiniGameGameInstanceSubsystem* MiniGameGameSubsystem = GetSubsystem<UMiniGameGameInstanceSubsystem>())
	{
		MiniGameGameSubsystem->ObjectPoolManagerClass = ObjectPoolManagerClass;
		MiniGameGameSubsystem->InitSubsystem();
	}
}

void UVNGameInstance::ResetMiniGameSubSystem() const
{
	if (const UMiniGameGameInstanceSubsystem* MiniGameGameSubsystem = GetSubsystem<UMiniGameGameInstanceSubsystem>())
	{
		MiniGameGameSubsystem->ClearPoolManager();
	}
}
