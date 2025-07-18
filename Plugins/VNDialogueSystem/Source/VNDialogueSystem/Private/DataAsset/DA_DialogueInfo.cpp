// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "DataAsset/DA_DialogueInfo.h"

FText UDA_DialogueInfo::FindCharacterNameById(const FName& CharacterId) const
{
	for (const FDialogueVisualInfo& Visual : DialogueVisuals)
	{
		if (Visual.CharacterID == CharacterId)
		{
			return Visual.CharacterName;
		}
	}

	return FText();
}

UTexture2D* UDA_DialogueInfo::FindCharacterTexture(const FName& CharacterId, EDialogueExpression Expression) const
{
	for (const FDialogueVisualInfo& Visual : DialogueVisuals)
	{
		if (Visual.CharacterID == CharacterId)
		{
			return *Visual.Expressions.Find(Expression);
		}
	}

	return nullptr;
}

TSoftObjectPtr<USoundBase> UDA_DialogueInfo::FindSFXByEnum(const EDialogueSFX SoundEffect) const
{
	if (SoundEffect == EDialogueSFX::None)
	{
		return nullptr;
	}

	return *DialogueSFX.Find(SoundEffect);
}

TSoftObjectPtr<USoundBase> UDA_DialogueInfo::FindBGMByEnum(const EDialogueBGM BGM) const
{
	if (BGM == EDialogueBGM::None)
	{
		return nullptr;
	}
	return *DialogueBGM.Find(BGM);
}

TSoftObjectPtr<UTexture2D> UDA_DialogueInfo::FindBackgroundImageByEnum(const EDialogueBGImage BGImage) const
{
	if (BGImage == EDialogueBGImage::None)
	{
		return nullptr;
	}
	return *DialogueBGImage.Find(BGImage);
}

TSoftObjectPtr<UTexture2D> UDA_DialogueInfo::FindCutSceneByEnum(const EDialogueCutScene CutScene) const
{
	if (CutScene == EDialogueCutScene::None)
	{
		return nullptr;
	}
	return *DialogueCutScene.Find(CutScene);
}

TSoftObjectPtr<UWorld> UDA_DialogueInfo::FindMiniGameWorld(const EMiniGameType MiniGameType, const int32 GameLevel) const
{
	if (MiniGameType == EMiniGameType::None)
	{
		return nullptr;
	}

	for (const FMiniGameMapInfo& MiniGameMapInfo : MiniGameData)
	{
		if (MiniGameMapInfo.MiniGameType == MiniGameType && MiniGameMapInfo.Level == GameLevel)
		{
			return MiniGameMapInfo.MiniGameSoftMap;
		}
	}

	return nullptr;
	
}
