// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueDebugHelper.h"
#include "VNDialogueGraph.generated.h"

class UDA_DialogueInfo;
class UVNDialogueNodeBase;
/**
 * DialogueGraph 클래스는 Json을 읽어 파싱하여 노드들로 바꾸고 이 노드를 보관합니다.
 */
UCLASS()
class VNDIALOGUESYSTEM_API UVNDialogueGraph : public UObject
{
	GENERATED_BODY()

public:
	UVNDialogueNodeBase* GetNodeById(const FName& InNodeId);
	void LoadFromJSON(const FString& JsonFileName);

private:
	UPROPERTY()
	TMap<FName /* Node ID*/, UVNDialogueNodeBase* /* Node */ > NodeMap;
	
	void CreateTextNode(const TSharedPtr<FJsonObject>& NodeObject, const TSharedPtr<FJsonObject>& JsonObject);
	void CreateChoicesNode(const TSharedPtr<FJsonObject>& NodeObject);
	void CreateConditionNode(const TSharedPtr<FJsonObject>& NodeObject);
	void CreateMiniGameNode(const TSharedPtr<FJsonObject>& NodeObject);
	void CreateEndNode(const TSharedPtr<FJsonObject>& NodeObject);
	
	// String을 Enum으로 변환하는 템플릿 함수
	template<typename TEnum>
	static TEnum StringToEnum(const FString& EnumName);
	
};

template <typename TEnum>
TEnum UVNDialogueGraph::StringToEnum(const FString& EnumName)
{
	// 특정 enum 타입 T의 UEnum 객체를 반환
	const UEnum* EnumPtr = StaticEnum<TEnum>();
		
	if (!EnumPtr)
	{
		DialogueDebugHelper::Print(TEXT("StringToEnum: Cannot find UEnum for the given enum type."), FColor::Red);
		return TEnum(0);
	}

	// string에 매칭되는 enum value를 획득.
	int32 EnumValue = EnumPtr->GetValueByNameString(EnumName);
		
	if (EnumValue == INDEX_NONE)
	{
		DialogueDebugHelper::Print(
			FString::Printf(TEXT("StringToEnum: '%s' is not a valid name for enum %s."), *EnumName, *EnumPtr->GetName()),
			FColor::Red	
		);
		return TEnum(0);
	}

	//static cast로 enum값으로 변환
	return static_cast<TEnum>(EnumValue);
}