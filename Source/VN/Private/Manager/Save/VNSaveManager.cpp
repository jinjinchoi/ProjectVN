// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Manager/Save/VNSaveManager.h"

#include "Core/SaveGame/VNSaveGame.h"
#include "Kismet/GameplayStatics.h"

void UVNSaveManager::Init()
{
	LoadMetaData();
}


void UVNSaveManager::LoadMetaData()
{
	if (UGameplayStatics::DoesSaveGameExist(META_SLOT_NAME, 0))
	{
		MetaSaveGame = Cast<UVNSaveGame>(UGameplayStatics::LoadGameFromSlot(META_SLOT_NAME, 0));
	}
	else
	{
		MetaSaveGame = Cast<UVNSaveGame>(UGameplayStatics::CreateSaveGameObject(UVNSaveGame::StaticClass()));
	}
}

void UVNSaveManager::SaveSoundVolume(const float MasterVolume, const float SFXVolume, const float BGMVolume, const float UIVolume)
{
	if (!MetaSaveGame)
	{
		LoadMetaData();
	}

	MetaSaveGame->MasterVolume = MasterVolume;
	MetaSaveGame->SFXVolume = SFXVolume;
	MetaSaveGame->BGMVolume = BGMVolume;
	MetaSaveGame->UIVolume = UIVolume;

	UGameplayStatics::SaveGameToSlot(MetaSaveGame, META_SLOT_NAME, 0);
	
}

void UVNSaveManager::SaveDialogueSpeedParams(const EDialogueSpeed DialoguePlaySpeed, const EDialogueSpeed SkipSpeed, const EDialogueSpeed AutoModeSpeed)
{
	if (!MetaSaveGame)
	{
		LoadMetaData();
	}

	MetaSaveGame->DialoguePlaySpeed = DialoguePlaySpeed;
	MetaSaveGame->SkipDialogueSpeed = SkipSpeed;
	MetaSaveGame->AutoModeSpeed = AutoModeSpeed;

	UGameplayStatics::SaveGameToSlot(MetaSaveGame, META_SLOT_NAME, 0);
	
}

void UVNSaveManager::SaveGameDataInSlot(const FString& SlotName, const int32 SlotIndex, const FSaveGameDataParams& SaveGameDataParams)
{
	UVNSaveGame* CurrentSaveGame = CastChecked<UVNSaveGame>(UGameplayStatics::CreateSaveGameObject(UVNSaveGame::StaticClass()));
	CurrentSaveGame->SlotName = SlotName;
	CurrentSaveGame->ChapterName = SaveGameDataParams.ChapterName;
	CurrentSaveGame->SavedTime = SaveGameDataParams.SavedTime;
	CurrentSaveGame->CharacterAffectionMap = SaveGameDataParams.CharacterAffectionMap;
	CurrentSaveGame->NodeName = SaveGameDataParams.NodeName;
	CurrentSaveGame->SelectedNodeIds = SaveGameDataParams.SelectedNodeIds;

	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, SlotIndex);
}


UVNSaveGame* UVNSaveManager::LoadGameDataInSlot(const FString& SlotName, const int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		return Cast<UVNSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex));
	}
	
	return nullptr;
}
