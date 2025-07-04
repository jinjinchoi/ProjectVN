// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Node/VNDialogueNodeBase.h"
#include "VNDialogueTextNode.generated.h"


struct FDialogueCharacterSetting
{
	FName CharacterID;
	EDialogueExpression CharacterExpression;
	EDialoguePosition DialoguePosition;
};

/**
 * 
 */
UCLASS()
class VNDIALOGUESYSTEM_API UVNDialogueTextNode : public UVNDialogueNodeBase
{
	GENERATED_BODY()

public:
	FName SpeakerID;
	FText DialogueText;
	TArray<FDialogueCharacterSetting> CharacterSettings;

	EDialogueSFX DialogueSFX;
	EDialogueBGM DialogueBGM;
	EDialogueBGImage DialogueBGImage;
	EDialogueCutScene DialogueCutScene;

	FName NextNodeId;

	FText ChapterName;
	
};
