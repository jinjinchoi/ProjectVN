// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once
#include "VNDialogueEnumType.h"
#include "VNDialogueStructType.generated.h"


USTRUCT(BlueprintType)
struct FCharacterSetting
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	EDialoguePosition DialoguePosition;

	UPROPERTY(BlueprintReadOnly)
	UTexture2D* CharacterImage;

	UPROPERTY(BlueprintReadOnly)
	bool bIsSpeaking;
	
};

USTRUCT(BlueprintType)
struct FTextDialogueInfo
{
	GENERATED_BODY()

	FTextDialogueInfo() {}

	explicit FTextDialogueInfo(const bool InIsValid)
		: bIsValid(InIsValid) {}
	
	UPROPERTY(BlueprintReadOnly)
	TArray<FCharacterSetting> CharacterSettings;

	UPROPERTY(BlueprintReadOnly)
	FText SpeakerName;
	
	UPROPERTY(BlueprintReadOnly)
	FText DialogueText;

	bool bPlaySFX = false;
	TSoftObjectPtr<USoundBase> SoundEffect;

	bool bPlayBGM = false;
	TSoftObjectPtr<USoundBase> BGM;
	
	UPROPERTY(BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> BGImage;

	UPROPERTY(BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> CutScene;

	EDialogueCutScene CutSceneEnum;
	
	FName NextNodeId;

	UPROPERTY(BlueprintReadOnly)
	FText ChapterName;

	bool bIsValid = true;
	
};

USTRUCT(BlueprintType)
struct FChoiceNodeInfo
{
	GENERATED_BODY()
	
	TSet<FName> RequiredNodes = TSet<FName>();
	int32 RequiredAffection = 0;

	UPROPERTY(BlueprintReadOnly)
	FName RelatedCharacterId = FName();

	UPROPERTY(BlueprintReadOnly)
	FText ChoiceText = FText();

	UPROPERTY(BlueprintReadOnly)
	int32 AffectionGain = 0;

	UPROPERTY(BlueprintReadOnly)
	FName NextNodeId = FName();

	UPROPERTY(BlueprintReadOnly)
	bool bIsConditionMet = true;
	
};

struct FConditionNodeInfo
{
	FConditionNodeInfo() {}
	explicit FConditionNodeInfo(const bool InbIsValid)
		: bIsValid(InbIsValid) {}
	
	TSet<FName> RequiredNodes = TSet<FName>();
	int32 RequiredAffection = 0;
	FName RelatedCharacterId = NAME_None; 
	FName TrueNodeId = NAME_None;
	FName FalseNodeId = NAME_None;

	bool bIsValid = true;
};

struct FMiniGameNodeInfo
{
	FMiniGameNodeInfo() {}
	explicit FMiniGameNodeInfo(const bool InbIsValid)
		: bIsValid(InbIsValid) {}
	
	TSoftObjectPtr<UWorld> MiniGameMap = nullptr;
	FName NodeIdAfterClear = NAME_None;

	bool bIsValid = true;
	
};