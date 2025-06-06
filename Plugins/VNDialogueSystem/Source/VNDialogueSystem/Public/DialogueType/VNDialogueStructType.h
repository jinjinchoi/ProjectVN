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
	{
		bIsValid = InIsValid;
	}

	UPROPERTY(BlueprintReadOnly)
	TArray<FCharacterSetting> CharacterSettings;

	UPROPERTY(BlueprintReadOnly)
	FText SpeakerName;
	
	UPROPERTY(BlueprintReadOnly)
	FText DialogueText;

	UPROPERTY(BlueprintReadOnly)
	TSoftObjectPtr<USoundBase> BGM;

	UPROPERTY(BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> BGImage;

	UPROPERTY(BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> CutScene;
	
	FName NextNodeId;

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