// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VNDialogueEnumType.h"
#include "VNDialogueStructType.h"
#include "VNDialogueManager.generated.h"

class UVNDialogueNodeBase;
class UDA_DialogueInfo;
class UVNDialogueGraph;


/**
 * 매니저 클래스는 그래프 클래스에서 노드들을 가져와 정보를 가공하고 이것을 UI로 보내는 역할을 합니다.
 */
UCLASS(Blueprintable)
class VNDIALOGUESYSTEM_API UVNDialogueManager : public UObject
{
	GENERATED_BODY()

public:
	void Init();
	EDialogueType GetDialogueTypeById(const FName& NodeId) const;

	/* 각각의 노드들에서 필요한 정보를 반환하는 함수 */
	FTextDialogueInfo GetTextDialogueInfoById(const FName& NodeId) const;
	TArray<FChoiceNodeInfo> GetChoiceNodeInfoById(const FName& NodeId) const;
	FConditionNodeInfo GetConditionNodeInfoById(const FName& NodeId) const;
	FMiniGameNodeInfo GetMiniGameNodeInfoById(const FName& NodeId) const;

protected:
	/* 파싱할 JSON 파일 이름 */
	UPROPERTY(EditDefaultsOnly)
	TArray<FString> DialogueJsonFileNames;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDA_DialogueInfo> DialogueInfoAsset;

private:
	UPROPERTY()
	TObjectPtr<UVNDialogueGraph> DialogueGraph;
	
	UVNDialogueNodeBase* GetDialogueNodeById(const FName& NodeId) const;
	
};

// TODO : 메타 정보 받아와서 챕터 변경 여부 알리는 델리게이트 만들 필요 있어보임.