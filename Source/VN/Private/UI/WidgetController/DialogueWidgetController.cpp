// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "UI/WidgetController/DialogueWidgetController.h"

#include "DebugHelper.h"
#include "VNDialogueManager.h"
#include "Core/GameInstance/VNGameInstance.h"
#include "Core/SaveGame/VNSaveGame.h"
#include "Manager/Save/VNSaveManager.h"
#include "Manager/Sound/VNSoundManager.h"
#include "Player/VNPlayerState.h"

void UDialogueWidgetController::Init()
{
	LoadDialogueSpeedData();
}

void UDialogueWidgetController::BroadCastDialog()
{
	if (!DialogueManager)
	{
		DialogueManager = CastChecked<UVNGameInstance>(GetWorld()->GetGameInstance())->GetDialogueManager();
	}

	const EDialogueType DialogueType = DialogueManager->GetDialogueTypeById(DialogueID);

	switch (DialogueType)
	{
		case EDialogueType::Dialogue:
			HandleTextDialogue(DialogueManager);
			break;
		
		case EDialogueType::Choice:
			HandleChoiceNode(DialogueManager);
			break;
		
		case EDialogueType::Condition:
			HandleConditionNode(DialogueManager);
			break;
		
		case EDialogueType::MiniGame:
			HandleMiniGameNode(DialogueManager);
		
		default:
			break;
	}
	
}


void UDialogueWidgetController::OnChoiceButtonClicked(const FName& NextNodeId, const FText& ChosenText, const FName RelatedCharacterId, const int32 AffectionToIncrease)
{
	AVNPlayerState* PlayerState = PlayerController->GetPlayerState<AVNPlayerState>();
	if (!PlayerState)
	{
		DebugHelper::Print(FString::Printf(TEXT("Invalid PlayerState detected in function: %hs"), __FUNCTION__), FColor::Red);
		return;
	}
	
	if (RelatedCharacterId != NAME_None && AffectionToIncrease != 0)
	{
		PlayerState->AddAffectionToCharacter(RelatedCharacterId, AffectionToIncrease);
		PlayerState->AddSelectedNodeId(NextNodeId);
	}

	PlayerState->AddSelectedNodeId(NextNodeId);
	AddToBackLog(ENodeType::Choice, FText(),ChosenText);
	
	DialogueID = NextNodeId;
	BroadCastDialog();
}

void UDialogueWidgetController::LoadDialogueSpeedData()
{
	UVNGameInstance* GameInstance = CastChecked<UVNGameInstance>(PlayerController->GetGameInstance());

	UVNSaveManager* SaveManager = GameInstance->GetSaveManager();
	if (!SaveManager) return;
	
	SaveManager->LoadMetaData();

	if (const UVNSaveGame* SaveGame = SaveManager->GetMetaSaveGame())
	{
		DialoguePlaySpeed = SaveGame->DialoguePlaySpeed;
		SkipDialogueSpeed = SaveGame->SkipDialogueSpeed;
		AutoModePlaySpeed = SaveGame->AutoModeSpeed;
	}
}

void UDialogueWidgetController::SaveDialogueSpeedData() const
{
	const UVNGameInstance* GameInstance = CastChecked<UVNGameInstance>(PlayerController->GetGameInstance());

	if (UVNSaveManager* SaveManager = GameInstance->GetSaveManager())
	{
		SaveManager->SaveDialogueSpeedParams(DialoguePlaySpeed, SkipDialogueSpeed, AutoModePlaySpeed);
	}
	
}

FLoadSlotUInfo UDialogueWidgetController::GetSlotUIInfo(const FString& SlotName, const int32 SlotIndex) const
{
	check(PlayerController)

	const UVNGameInstance* GameInstance = Cast<UVNGameInstance>(PlayerController->GetGameInstance());
	if (GameInstance == nullptr)
	{
		return FLoadSlotUInfo();
	}

	if (UVNSaveManager* SaveManager = GameInstance->GetSaveManager())
	{
		if (const UVNSaveGame* LoadedGameData = SaveManager->LoadGameDataInSlot(SlotName, SlotIndex))
		{
			FLoadSlotUInfo UIData;
			UIData.ChapterName = LoadedGameData->ChapterName;
			UIData.SavedTime = LoadedGameData->SavedTime;

			return UIData;
		}
	}

	return FLoadSlotUInfo();
}

bool UDialogueWidgetController::SaveDialogueDataInSlot(const FString& SlotName, const int32 SlotIndex)
{
	check(PlayerController);
	
	const UVNGameInstance* GameInstance = CastChecked<UVNGameInstance>(PlayerController->GetGameInstance());
	const AVNPlayerState* PlayerState = PlayerController->GetPlayerState<AVNPlayerState>();
	if (!PlayerState || !GameInstance) return false;

	if (UVNSaveManager* SaveManager = GameInstance->GetSaveManager())
	{
		FSaveGameDataParams SaveGameDataParams;
		SaveGameDataParams.ChapterName = ChapterName;
		SaveGameDataParams.NodeName = LastDialogueID;
		SaveGameDataParams.SavedTime = GetCurrentTimeText();
		SaveGameDataParams.CharacterAffectionMap = PlayerState->GetAffectionMap();
		SaveGameDataParams.SelectedNodeIds = PlayerState->GetSelectedNodeIds();
		
		SaveManager->SaveGameDataInSlot(SlotName, SlotIndex, SaveGameDataParams);

		return true;
	}

	return false;
}

void UDialogueWidgetController::HandleTextDialogue(const UVNDialogueManager* InDialogueManager)
{
	const FTextDialogueInfo TextDialogueInfo = InDialogueManager->GetTextDialogueInfoById(DialogueID);
	AddToBackLog(ENodeType::Dialogue, TextDialogueInfo.SpeakerName, TextDialogueInfo.DialogueText);
	
	if (TextDialogueInfo.bIsValid)
	{
		DialogueID = TextDialogueInfo.NextNodeId;
		TextDialogueInfoReceivedDelegate.Broadcast(TextDialogueInfo);
		LastDialogueID = DialogueID;
		ChapterName = TextDialogueInfo.ChapterName;
	}

	// Sound 재생
	TextDialogueInfo.bPlayBGM ? PlayBackGroundMusic(TextDialogueInfo.BGM) : StopBackGroundMusic();
	TextDialogueInfo.bPlaySFX ? PlaySoundEffect(TextDialogueInfo.SoundEffect) : StopSoundEffect();

	// 컷신 저장
	if (TextDialogueInfo.CutSceneEnum != EDialogueCutScene::None)
	{
		AVNPlayerState* PlayerState = PlayerController->GetPlayerState<AVNPlayerState>();
		if (!PlayerState)
		{
			DebugHelper::Print(FString::Printf(TEXT("Invalid PlayerState detected in function: %hs"), __FUNCTION__), FColor::Red);
			return;
		}
	
		PlayerState->AddSeenCutScene(TextDialogueInfo.CutSceneEnum);
		// TODO : 컷신 세이브 파일에 저장해야함.
	}

}

void UDialogueWidgetController::HandleChoiceNode(const UVNDialogueManager* InDialogueManager) const
{
	TArray<FChoiceNodeInfo> ChoiceNodeInfos = InDialogueManager->GetChoiceNodeInfoById(DialogueID);
	if (ChoiceNodeInfos.IsEmpty()) return;

	for (FChoiceNodeInfo& ChoiceNodeInfo : ChoiceNodeInfos)
	{
		ChoiceNodeInfo.bIsConditionMet = IsConditionSatisfied(ChoiceNodeInfo.RelatedCharacterId, ChoiceNodeInfo.RequiredAffection, ChoiceNodeInfo.RequiredNodes);
	}

	ChoiceNodeInfoReceivedDelegate.Broadcast(ChoiceNodeInfos);
}

void UDialogueWidgetController::HandleConditionNode(const UVNDialogueManager* InDialogueManager)
{
	FConditionNodeInfo ConditionNodeInfo = InDialogueManager->GetConditionNodeInfoById(DialogueID);
	if (!ConditionNodeInfo.bIsValid) return;

	if (IsConditionSatisfied(ConditionNodeInfo.RelatedCharacterId, ConditionNodeInfo.RequiredAffection, ConditionNodeInfo.RequiredNodes))
	{
		DialogueID = ConditionNodeInfo.TrueNodeId;
	}
	else
	{
		DialogueID = ConditionNodeInfo.FalseNodeId;
	}

	BroadCastDialog();
	
}

void UDialogueWidgetController::HandleMiniGameNode(const UVNDialogueManager* InDialogueManager)
{
	FMiniGameNodeInfo MiniGameNodeInfo = InDialogueManager->GetMiniGameNodeInfoById(DialogueID);

	// TODO :: Map 로딩 구현해야함.
	// 맵 이동은 별도의 클래스에서
}

bool UDialogueWidgetController::IsConditionSatisfied(const FName& RelatedCharacterId, const int32 RequiredAffection, const TSet<FName>& RequiredNodes) const
{
	check(PlayerController);
	
	const AVNPlayerState* PlayerState = PlayerController->GetPlayerState<AVNPlayerState>();
	if (!PlayerState)
	{
		DebugHelper::Print(FString::Printf(TEXT("Invalid PlayerState detected in function: %hs"), __FUNCTION__), FColor::Red);
		return false;
	}
	
	if (0 != RequiredAffection && PlayerState->GetCharacterAffection(RelatedCharacterId) < RequiredAffection)
	{
		return false;
	}

	for (const FName& RequiredNode : RequiredNodes)
	{
		if (!PlayerState->GetSelectedNodeIds().Contains(RequiredNode))
		{
			return false;
		}
	}

	return true;
}

void UDialogueWidgetController::AddToBackLog(const ENodeType NodeType, const FText& CharacterName, const FText& LogText)
{
	const FBackLogParam LogParam(NodeType, CharacterName, LogText);
	
	if (BackLogParams.Num() < BackLogSize)
	{
		BackLogParams.Add(LogParam);
	}
	else
	{
		BackLogParams[BackLogHeadIndex] = LogParam;
		BackLogHeadIndex = (BackLogHeadIndex + 1) % BackLogSize;
	}
}

void UDialogueWidgetController::PlayBackGroundMusic(const TSoftObjectPtr<USoundBase>& BGMToPlay) const
{
	if (BGMToPlay.IsNull()) return;

	if (UVNSoundManager* SoundManager = CastChecked<UVNGameInstance>(GetWorld()->GetGameInstance())->GetSoundManager())
	{
		SoundManager->PlayBackGroundMusic(BGMToPlay);
	}
}

void UDialogueWidgetController::PlaySoundEffect(const TSoftObjectPtr<USoundBase>& SFXToPlay) const
{
	if (SFXToPlay.IsNull()) return;

	if (UVNSoundManager* SoundManager = CastChecked<UVNGameInstance>(GetWorld()->GetGameInstance())->GetSoundManager())
	{
		SoundManager->PlaySFX(SFXToPlay);
	}
}

void UDialogueWidgetController::StopBackGroundMusic() const
{
	if (const UVNSoundManager* SoundManager = CastChecked<UVNGameInstance>(GetWorld()->GetGameInstance())->GetSoundManager())
	{
		SoundManager->StopBackGroundMusic();
	}
}

void UDialogueWidgetController::StopSoundEffect() const
{
	if (const UVNSoundManager* SoundManager = CastChecked<UVNGameInstance>(GetWorld()->GetGameInstance())->GetSoundManager())
	{
		SoundManager->StopSFX();
	}
}

FText UDialogueWidgetController::GetCurrentTimeText() const
{
	const FDateTime Now = FDateTime::Now();

	FString FormattedTime = FString::Printf(TEXT("%d년 %02d월 %02d일 %02d시 %02d분 %02d초"),
		Now.GetYear(),
		Now.GetMonth(),
		Now.GetDay(),
		Now.GetHour(),
		Now.GetMinute(),
		Now.GetSecond());

	return FText::FromString(FormattedTime);
}
