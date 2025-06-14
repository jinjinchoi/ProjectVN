// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "VN/Public/Core/GameInstance/VNGameInstance.h"

#include "VNDialogueManager.h"
#include "Core/SaveGame/VNSaveGame.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/Save/VNSaveManager.h"
#include "Manager/Sound/VNSoundManager.h"


void UVNGameInstance::Init()
{
	Super::Init();

	check(DialogueManagerClass && SoundManagerClass)

	CreateManagerClasses();
	SetUserConfig();
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