// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "VNDialogueManager.h"

#include "DataAsset/DA_DialogueInfo.h"
#include "Graph/VNDialogueGraph.h"
#include "Node/VNDialogueChoiceNode.h"
#include "Node/VNDialogueConditionNode.h"
#include "Node/VNDialogueNodeBase.h"
#include "Node/VNDialogueTextNode.h"
#include "Node/VNMiniGameNode.h"


void UVNDialogueManager::Init()
{
	if (DialogueJsonFileNames.IsEmpty()) return;


	DialogueGraph = NewObject<UVNDialogueGraph>(this);

	for (const FString& FileName : DialogueJsonFileNames)
	{
		DialogueGraph->LoadFromJSON(FileName);
	}
}

UVNDialogueNodeBase* UVNDialogueManager::GetDialogueNodeById(const FName& NodeId) const
{
	if (DialogueGraph)
	{
		return DialogueGraph->GetNodeById(NodeId);
	}

	DialogueDebugHelper::Print(FString::Printf(TEXT("%hs: InValid Node ID: %s"),  __FUNCTION__, *NodeId.ToString()), FColor::Yellow);
	return nullptr;
}


EDialogueType UVNDialogueManager::GetDialogueTypeById(const FName& NodeId) const
{
	if (UVNDialogueNodeBase* Node = GetDialogueNodeById(NodeId))
	{
		return Node->NodeType;
	}

	return EDialogueType::None;
}

FTextDialogueInfo UVNDialogueManager::GetTextDialogueInfoById(const FName& NodeId) const
{
	check(DialogueInfoAsset);
	
	UVNDialogueNodeBase* Node = GetDialogueNodeById(NodeId);
	if (!Node)
	{
		return FTextDialogueInfo(false);
	}

	UVNDialogueTextNode* TextNode = CastChecked<UVNDialogueTextNode>(Node);

	FTextDialogueInfo DialogueInfo;
	for (const FDialogueCharacterSetting& Setting : TextNode->CharacterSettings)
	{
		FCharacterSetting CharacterSetting;
		CharacterSetting.CharacterImage = DialogueInfoAsset->FindCharacterTexture(Setting.CharacterID, Setting.CharacterExpression);
		CharacterSetting.bIsSpeaking = TextNode->SpeakerID == Setting.CharacterID; 
		CharacterSetting.DialoguePosition = Setting.DialoguePosition;

		DialogueInfo.CharacterSettings.Add(CharacterSetting);
	}

	DialogueInfo.SpeakerName = DialogueInfoAsset->FindCharacterNameById(TextNode->SpeakerID);
	DialogueInfo.DialogueText = TextNode->DialogueText;
	DialogueInfo.BGImage = DialogueInfoAsset->FindBackgroundImageByEnum(TextNode->DialogueBGImage);
	DialogueInfo.CutScene = DialogueInfoAsset->FindCutSceneByEnum(TextNode->DialogueCutScene);
	DialogueInfo.CutSceneEnum = TextNode->DialogueCutScene;

	if (TextNode->DialogueBGM != EDialogueBGM::Stop)
	{
		DialogueInfo.bPlayBGM = true;
		DialogueInfo.BGM = DialogueInfoAsset->FindBGMByEnum(TextNode->DialogueBGM);
	}
	
	if (TextNode->DialogueSFX != EDialogueSFX::Stop)
	{
		DialogueInfo.bPlaySFX = true;
		DialogueInfo.SoundEffect = DialogueInfoAsset->FindSFXByEnum(TextNode->DialogueSFX);
	}
	
	DialogueInfo.NextNodeId = TextNode->NextNodeId;
	DialogueInfo.ChapterName = TextNode->ChapterName;

	return DialogueInfo;
	
}

TArray<FChoiceNodeInfo> UVNDialogueManager::GetChoiceNodeInfoById(const FName& NodeId) const
{
	UVNDialogueNodeBase* Node = GetDialogueNodeById(NodeId);
	if (!Node)
	{
		TArray<FChoiceNodeInfo> ChoiceNodeInfos;
		return ChoiceNodeInfos;
	}

	UVNDialogueChoiceNode* ChoiceNode = CastChecked<UVNDialogueChoiceNode>(Node);
	return ChoiceNode->DialogueChoices;

	
}

FConditionNodeInfo UVNDialogueManager::GetConditionNodeInfoById(const FName& NodeId) const
{
	UVNDialogueNodeBase* Node = GetDialogueNodeById(NodeId);
	if (!Node)
	{
		return FConditionNodeInfo(false);
	}

	UVNDialogueConditionNode* ConditionNode = CastChecked<UVNDialogueConditionNode>(Node);
	return ConditionNode->ConditionNodeInfo;
	
}

FMiniGameNodeInfo UVNDialogueManager::GetMiniGameNodeInfoById(const FName& NodeId) const
{
	UVNDialogueNodeBase* Node = GetDialogueNodeById(NodeId);
	if (!Node)
	{
		return FMiniGameNodeInfo(false);
	}

	const UVNMiniGameNode* MiniGameNode = CastChecked<UVNMiniGameNode>(Node);
	FMiniGameNodeInfo MiniGameInfo;
	MiniGameInfo.MiniGameName = MiniGameNode->MiniGameName;
	MiniGameInfo.MiniGameMap = DialogueInfoAsset->FindMiniGameWorld(MiniGameNode->MiniGameType, MiniGameNode->MiniGameLevel);
	MiniGameInfo.NodeIdAfterClear = MiniGameNode->NodeIdAfterClear;

	return MiniGameInfo;
	
}
