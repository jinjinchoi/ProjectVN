// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once
#include "VNDialogueEnumType.h"
#include "VNDialogueStructType.generated.h"


USTRUCT(BlueprintType)
struct FCharacterSetting
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	FText CharacterName;
	
	UPROPERTY(BlueprintReadOnly)
	EDialoguePosition DialoguePosition;

	UPROPERTY(BlueprintReadOnly)
	UTexture2D* CharacterImage;
	
};

USTRUCT(BlueprintType)
struct FTextDialogueInfo
{
	GENERATED_BODY()

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
	
};