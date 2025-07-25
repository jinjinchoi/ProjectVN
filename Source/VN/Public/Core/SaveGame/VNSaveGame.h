// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "VNDialogueEnumType.h"
#include "VNSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class VN_API UVNSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

#pragma region Slot
	
	UPROPERTY()
	FString SlotName = FString();

#pragma endregion


//////////////////////////////////////////////////////////////
/* Main Slot에 저장할 요소 (모든 게임에 필요한 데이터) */
//////////////////////////////////////////////////////////////
	
#pragma region Meta

	UPROPERTY()
	TSet<EDialogueCutScene> SeenCutScenes = TSet<EDialogueCutScene>();

#pragma endregion 


#pragma region DialogueSpeed

	UPROPERTY()
	EDialogueSpeed DialoguePlaySpeed = EDialogueSpeed::Normal;
	
	UPROPERTY()
	EDialogueSpeed SkipDialogueSpeed = EDialogueSpeed::Slow;
	
	UPROPERTY()
	EDialogueSpeed AutoModeSpeed = EDialogueSpeed::Normal;

#pragma endregion

	
#pragma region Sound

	UPROPERTY()
	float MasterVolume = 1.f;

	UPROPERTY()
	float BGMVolume = 1.f;

	UPROPERTY()
	float SFXVolume = 1.f;

	UPROPERTY()
	float UIVolume = 1.f;

#pragma endregion
	
	
//////////////////////////////////////////////////////////////
/* 개별 슬롯에 저장할 요소 (각각의 슬롯에 필요한 데이터) */
//////////////////////////////////////////////////////////////

#pragma  region SlotData

	UPROPERTY()
	FText ChapterName = FText();
	
	UPROPERTY()
	FName NodeName = FName();

	UPROPERTY()
	TSet<FName> SelectedNodeIds = TSet<FName>();

	UPROPERTY()
	TMap<FName, int32> CharacterAffectionMap = TMap<FName, int32>();
	
	UPROPERTY()
	FText SavedTime = FText();
	
#pragma endregion


};
