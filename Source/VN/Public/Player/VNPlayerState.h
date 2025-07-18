// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VNDialogueEnumType.h"
#include "GameFramework/PlayerState.h"
#include "VNPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class VN_API AVNPlayerState : public APlayerState
{
	GENERATED_BODY()

#pragma region  VisualNobleParameters
	
public:
	// 호감도 설정
	int32 GetCharacterAffection(const FName& CharacterId) const;
	void SetCharacterAffectionMap(const TMap<FName, int32>& InCharacterAffectionMap);
	void AddAffectionToCharacter(const FName& CharacterId , const int32 AffectionToAdd);

	// 선택된 노드 설정
	TSet<FName> GetSelectedNodeIds() const;
	void SetSelectedNodeIds(const TSet<FName>& InSelectedNodeIds);
	void AddSelectedNodeId(const FName& SelectedNode);

	// 한번 본 컷신 설정
	TSet<EDialogueCutScene> GetSeenCutScenes() const;
	void SetSeenCutScenes(const TSet<EDialogueCutScene>& InSeenCutScenes);
	void AddSeenCutScene(const EDialogueCutScene InSeenCutScene);
	
private:
	TSet<FName> SelectedNodeIds;
	TMap<FName /* Character ID */, int32 /* Affection */> CharacterAffectionMap;

	// TODO: 위치 바꿔야함. 전체 데이터에 저장할 데이터임.
	TSet<EDialogueCutScene> SeenCutScenes;

public:
	FORCEINLINE TMap<FName, int32> GetAffectionMap() const { return CharacterAffectionMap; }

#pragma endregion 
	
};
