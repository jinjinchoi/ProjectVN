// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Player/VNPlayerState.h"

#pragma region  VisualNobleParameters

void AVNPlayerState::SetCharacterAffectionMap(const TMap<FName, int32>& InCharacterAffectionMap)
{
	CharacterAffectionMap = InCharacterAffectionMap;
}

void AVNPlayerState::SetSelectedNodeIds(const TSet<FName>& InSelectedNodeIds)
{
	SelectedNodeIds = InSelectedNodeIds;
}

void AVNPlayerState::AddAffectionToCharacter(const FName& CharacterId , const int32 AffectionToAdd)
{
	if (CharacterAffectionMap.Contains(CharacterId))
	{
		CharacterAffectionMap[CharacterId] += AffectionToAdd;
	}
	else
	{
		CharacterAffectionMap.Add(CharacterId, AffectionToAdd);
	}
}

void AVNPlayerState::AddSelectedNodeId(const FName& SelectedNode)
{
	SelectedNodeIds.Add(SelectedNode);
}

TSet<EDialogueCutScene> AVNPlayerState::GetSeenCutScenes() const
{
	return SeenCutScenes;
}

void AVNPlayerState::SetSeenCutScenes(const TSet<EDialogueCutScene>& InSeenCutScenes)
{
	SeenCutScenes = InSeenCutScenes;
}

void AVNPlayerState::AddSeenCutScene(const EDialogueCutScene InSeenCutScene)
{
	SeenCutScenes.Add(InSeenCutScene);
}

int32 AVNPlayerState::GetCharacterAffection(const FName& CharacterId) const
{
	const int32* FoundAffection = CharacterAffectionMap.Find(CharacterId);
	return FoundAffection ? *FoundAffection : 0;
}

TSet<FName> AVNPlayerState::GetSelectedNodeIds() const
{
	return SelectedNodeIds;
}

#pragma endregion