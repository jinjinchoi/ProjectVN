// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Graph/VNDialogueGraph.h"

#include "VNDialogueStructType.h"
#include "Node/VNDialogueChoiceNode.h"
#include "Node/VNDialogueConditionNode.h"
#include "Node/VNDialogueEndNode.h"
#include "Node/VNDialogueTextNode.h"
#include "Node/VNMiniGameNode.h"

UVNDialogueNodeBase* UVNDialogueGraph::GetNodeById(const FName& InNodeId)
{
	if (NodeMap.Find(InNodeId))
	{
		return *NodeMap.Find(InNodeId);
	}

	return nullptr;
}

void UVNDialogueGraph::LoadFromJSON(const FString& JsonFileName)
{
	const FString FilePath = FPaths::ProjectDir() / TEXT("Dialogue") / (JsonFileName + TEXT(".json"));
	FString JsonRaw;
	if (!FFileHelper::LoadFileToString(JsonRaw, *FilePath))
	{
		DialogueDebugHelper::Print(FString::Printf(TEXT("Failed to load JSON file: %s"), *FilePath), FColor::Red);
		return;
	}
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonRaw);

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		const TArray<TSharedPtr<FJsonValue>> NodeArray = JsonObject->GetArrayField(TEXT("nodes"));

		for (const TSharedPtr<FJsonValue>& Node : NodeArray)
		{
			TSharedPtr<FJsonObject> NodeObject = Node->AsObject();
			FString Type = NodeObject->GetStringField(TEXT("type"));
			
			// 다이얼로그 타입에 따라 해당하는 노드 생성
			if (Type == TEXT("dialogue"))
			{
				CreateTextNode(NodeObject, JsonObject);
			}
			else if (Type == TEXT("choice"))
			{
				CreateChoicesNode(NodeObject);
			}
			else if (Type == TEXT("condition"))
			{
				CreateConditionNode(NodeObject);
			}
			else if (Type == TEXT("miniGame"))
			{
				CreateMiniGameNode(NodeObject);
			}
			else if (Type == TEXT("end"))
			{
				CreateEndNode(NodeObject);
			}
		}
		
	}
	DialogueDebugHelper::Print(FString::Printf(TEXT("Success to load JSON file: %s"), *FilePath), FColor::Green);
	
}


void UVNDialogueGraph::CreateTextNode(const TSharedPtr<FJsonObject>& NodeObject, const TSharedPtr<FJsonObject>& JsonObject)
{
	
	// 대화 노드 생성
	UVNDialogueTextNode* TextNode = NewObject<UVNDialogueTextNode>();

	// 메타 데이터 값 저장
	const TSharedPtr<FJsonObject> MetaObject = JsonObject->GetObjectField(TEXT("meta"));
	const FString ChapterName = MetaObject->GetStringField(TEXT("chapterName"));
	TextNode->ChapterName = FText::FromString(ChapterName);
	
	// 노드 데이터 값 저장
	
	const FString NodeId = NodeObject->GetStringField(TEXT("id"));
	TextNode->NodeID = FName(*NodeId);
	
	const FString NextNodeId = NodeObject->GetStringField(TEXT("nextNodeId"));
	TextNode->NextNodeId = FName(*NextNodeId);
	
	TextNode->NodeType = EDialogueType::Dialogue;

	const FString SpeakerIdStr = NodeObject->GetStringField(TEXT("speakerId"));
	TextNode->SpeakerID = FName(*SpeakerIdStr);
	
	// 각각의 필드에서 값들 가져오는 작업 실시
	const FString DialogueStr = NodeObject->GetStringField(TEXT("dialogue"));
	TextNode->DialogueText = FText::FromString(DialogueStr);
	
	const FString BGImageStr = NodeObject->GetStringField(TEXT("BGImage"));
	TextNode->DialogueBGImage = StringToEnum<EDialogueBGImage>(BGImageStr);

	const FString BGMStr = NodeObject->GetStringField(TEXT("BGM"));
	TextNode->DialogueBGM = StringToEnum<EDialogueBGM>(BGMStr);

	const FString SoundEffectStr = NodeObject->GetStringField(TEXT("soundEffect"));
	TextNode->DialogueSFX = StringToEnum<EDialogueSFX>(SoundEffectStr);

	const FString CutSceneStr = NodeObject->GetStringField(TEXT("cutScene"));
	TextNode->DialogueCutScene = StringToEnum<EDialogueCutScene>(CutSceneStr);
	
	// 캐릭터 세팅 값(배열) 가져오고 순회하여 값 할당. 
	const TArray<TSharedPtr<FJsonValue>>& CharacterSettingsFromJson = NodeObject->GetArrayField(TEXT("characterSettings"));
	for (const TSharedPtr<FJsonValue>& CharacterSettingValue : CharacterSettingsFromJson)
	{
		// 캐릭터 세팅에서 Value Array를 오브젝트로 변환. Value안에는 캐릭터 Id, 감정, 위치가 있음
		const TSharedPtr<FJsonObject> CharacterSettingObject = CharacterSettingValue->AsObject();
		const FName CharacterID = FName(*CharacterSettingObject->GetStringField(TEXT("characterId")));
		const FString ExpressionStr = CharacterSettingObject->GetStringField(TEXT("expression"));
		const FString PositionStr = CharacterSettingObject->GetStringField(TEXT("position"));
		
		FDialogueCharacterSetting CharacterSetting;
		CharacterSetting.CharacterID = CharacterID;
		CharacterSetting.CharacterExpression = StringToEnum<EDialogueExpression>(ExpressionStr);
		CharacterSetting.DialoguePosition = StringToEnum<EDialoguePosition>(PositionStr);

		TextNode->CharacterSettings.Add(CharacterSetting);
	}

	NodeMap.Add(FName(*NodeId), TextNode);
}

void UVNDialogueGraph::CreateChoicesNode(const TSharedPtr<FJsonObject>& NodeObject)
{
	// 선택지 노드 생성
	UVNDialogueChoiceNode* ChoiceNode = NewObject<UVNDialogueChoiceNode>();
	
	const FString NodeId = NodeObject->GetStringField(TEXT("id"));
	ChoiceNode->NodeID = FName(*NodeId);

	ChoiceNode->NodeType = EDialogueType::Choice;

	FChoiceNodeInfo ChoiceParams;

	// 선택지 배열을 가져오고 순회
	const TArray<TSharedPtr<FJsonValue>> ChoicesNodeArray = NodeObject->GetArrayField(TEXT("choices"));
	for (const TSharedPtr<FJsonValue>& Choice : ChoicesNodeArray)
	{
		// 배열안의 밸류를 오브젝트로 변환
		const TSharedPtr<FJsonObject> ChoiceObject = Choice->AsObject();

		// requiredNodes 배열로 값 받아오고 순회
		const TArray<TSharedPtr<FJsonValue>> RequiredNodesArray = ChoiceObject->GetArrayField(TEXT("requiredNodes"));
		for (const TSharedPtr<FJsonValue>& RequiredNode : RequiredNodesArray)
		{
			// 배열안의 밸류들 하나하나 가져와서 저장
			FString RequireNodeName = RequiredNode->AsString();
			ChoiceParams.RequiredNodes.Add(FName(*RequireNodeName));
		}

		// ChoiceObject 안에 있는 필드 값 가져와서 저장
		// 필요 호감도
		const int32 RequiredAffection = ChoiceObject->GetIntegerField(TEXT("requiredAffection"));
		ChoiceParams.RequiredAffection = RequiredAffection;

		// 선택지 내용
		const FString ChoiceText = ChoiceObject->GetStringField(TEXT("choiceText"));
		ChoiceParams.ChoiceText = FText::FromString(ChoiceText);

		// 선택지 선택시 획득 호감도
		const int32 AffectionGain = ChoiceObject->GetIntegerField(TEXT("affectionGain"));
		ChoiceParams.AffectionGain = AffectionGain;

		// 호감도가 오를 캐릭터
		const FString RelatedCharacter = ChoiceObject->GetStringField(TEXT("relatedCharacterId"));
		ChoiceParams.RelatedCharacterId = FName(*RelatedCharacter);

		// 선택시 선택시 이동할 노드
		const FString NextNodeId = ChoiceObject->GetStringField(TEXT("nextNodeId"));
		ChoiceParams.NextNodeId = FName(*NextNodeId);
		
		ChoiceNode->DialogueChoices.Add(ChoiceParams);
	}
	
	NodeMap.Add(FName(*NodeId), ChoiceNode);
	
}

void UVNDialogueGraph::CreateConditionNode(const TSharedPtr<FJsonObject>& NodeObject)
{
	UVNDialogueConditionNode* ConditionNode = NewObject<UVNDialogueConditionNode>();
	const FString NodeId = NodeObject->GetStringField(TEXT("id"));
	ConditionNode->NodeID = FName(*NodeId);

	ConditionNode->NodeType = EDialogueType::Condition;
	const TArray<TSharedPtr<FJsonValue>> RequiredNodeArray= NodeObject->GetArrayField(TEXT("requiredNodes"));
	for (const TSharedPtr<FJsonValue>& RequiredNode : RequiredNodeArray)
	{
		FString RequiredNodeName = RequiredNode->AsString();
		ConditionNode->ConditionNodeInfo.RequiredNodes.Add(FName(*RequiredNodeName));
	}

	const int32 RequiredAffection = NodeObject->GetIntegerField(TEXT("requiredAffection"));
	ConditionNode->ConditionNodeInfo.RequiredAffection = RequiredAffection;

	const FString RelatedCharacterId = NodeObject->GetStringField(TEXT("relatedCharacterId"));
	ConditionNode->ConditionNodeInfo.RelatedCharacterId = FName(*RelatedCharacterId);

	const FString TrueNodeStr = NodeObject->GetStringField(TEXT("trueNodeId"));
	ConditionNode->ConditionNodeInfo.TrueNodeId = FName(*TrueNodeStr);

	const FString FalseNodeStr = NodeObject->GetStringField(TEXT("falseNodeId"));
	ConditionNode->ConditionNodeInfo.FalseNodeId = FName(*FalseNodeStr);

	NodeMap.Add(FName(*NodeId), ConditionNode);
}

void UVNDialogueGraph::CreateMiniGameNode(const TSharedPtr<FJsonObject>& NodeObject)
{
	UVNMiniGameNode* MiniGameNode = NewObject<UVNMiniGameNode>();
	const FString NodeId = NodeObject->GetStringField(TEXT("id"));
	MiniGameNode->NodeID = FName(*NodeId);
	MiniGameNode->NodeType = EDialogueType::MiniGame;

	const FString MiniGameTypeStr = NodeObject->GetStringField(TEXT("miniGameType"));
	const EMiniGameType MiniGameType = StringToEnum<EMiniGameType>(MiniGameTypeStr);
	MiniGameNode->MiniGameType = MiniGameType;

	const int32 Level = NodeObject->GetIntegerField(TEXT("level"));
	MiniGameNode->MiniGameLevel = Level;

	const FString MiniGameNameStr = NodeObject->GetStringField(TEXT("miniGameName"));
	MiniGameNode->MiniGameName = FName(*MiniGameNameStr);

	const FString NodeIdAfterClearStr = NodeObject->GetStringField(TEXT("nodeIdAfterClear"));
	MiniGameNode->NodeIdAfterClear = FName(*NodeIdAfterClearStr);

	NodeMap.Add(FName(*NodeId), MiniGameNode);
	
}

void UVNDialogueGraph::CreateEndNode(const TSharedPtr<FJsonObject>& NodeObject)
{
	UVNDialogueEndNode* EndNode = NewObject<UVNDialogueEndNode>();
	const FString NodeId = NodeObject->GetStringField(TEXT("id"));
	EndNode->NodeID = FName(*NodeId);
	EndNode->NodeType = EDialogueType::End;

	NodeMap.Add(FName(*NodeId), EndNode);
	
}
