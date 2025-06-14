// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "UI/WidgetController/SettingWidgetController.h"

#include "Core/GameInstance/VNGameInstance.h"
#include "Core/SaveGame/VNSaveGame.h"
#include "Manager/Save/VNSaveManager.h"

void USettingWidgetController::LoadSoundDataIfExist()
{
	UVNGameInstance* GameInstance = CastChecked<UVNGameInstance>(PlayerController->GetGameInstance());

	UVNSaveManager* SaveManager = GameInstance->GetSaveManager();
	SaveManager->LoadMetaData();

	if (const UVNSaveGame* SaveGame = SaveManager->GetMetaSaveGame())
	{
		MasterVolume = SaveGame->MasterVolume;
		BGMVolume = SaveGame->BGMVolume;
		SFXVolume = SaveGame->SFXVolume;
		UIVolume = SaveGame->UIVolume;
	}
	
}

void USettingWidgetController::SaveSoundData()
{
	const UVNGameInstance* GameInstance = CastChecked<UVNGameInstance>(PlayerController->GetGameInstance());

	if (UVNSaveManager* SaveManager = GameInstance->GetSaveManager())
	{
		SaveManager->SaveSoundVolume(MasterVolume, SFXVolume, BGMVolume, UIVolume);
	}
}
