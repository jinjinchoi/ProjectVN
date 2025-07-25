// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VNDialogueEnumType.h"
#include "UI/WidgetController/VNWidgetControllerBase.h"
#include "DialogueWidgetController.generated.h"

/* 백로그에서 선택지 노드인지 대화 노드인지 구분하기 위한 enum */
UENUM(BlueprintType)
enum class ENodeType : uint8
{
	Dialogue,
	Choice
};

/* 백로그 정보를 담는 구조체 */
USTRUCT(BlueprintType)
struct FBackLogParam
{
	GENERATED_BODY()

	FBackLogParam()
		: NodeType(ENodeType::Dialogue), CharacterName(FText()), Log(FText()) {}
	
	FBackLogParam(const ENodeType InNodeType, const FText& InCharacterName, const FText& InText)
		: NodeType(InNodeType), CharacterName(InCharacterName), Log(InText) {}

	UPROPERTY(BlueprintReadOnly)
	ENodeType NodeType;

	UPROPERTY(BlueprintReadOnly)
	FText CharacterName;

	UPROPERTY(BlueprintReadOnly)
	FText Log;
	
};

USTRUCT(BlueprintType)
struct FLoadSlotUInfo
{
	GENERATED_BODY()

	FLoadSlotUInfo() {}
	
	UPROPERTY(BlueprintReadOnly)
	FText SavedTime = FText();
	
	UPROPERTY(BlueprintReadOnly)
	FText ChapterName = FText::FromString(TEXT("기록 없음"));

	UPROPERTY(BlueprintReadOnly)
	bool bHasSaveData = false;
	
};


class UVNDialogueManager;
struct FTextDialogueInfo;

/**
 * 다이얼로그 위젯 컨드롤러는 다이얼로그 시스템에서 대화 정보들을 가져와 위젯에 보내는 역할을 합니다.
 */
UCLASS()
class VN_API UDialogueWidgetController : public UVNWidgetControllerBase
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue Data")
	void BroadCastDialog();
	
	UPROPERTY(BlueprintAssignable, Category = "Dialogue Delegate")
	FOnTextDialogueInfoReceived TextDialogueInfoReceivedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Dialogue Delegate")
	FOnTChoiceNodeInfoReceived ChoiceNodeInfoReceivedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Dialogue Delegate")
	FOnEndNodeReceived EndNodeReceivedDelegate;


protected:
	UFUNCTION(BlueprintCallable, Category = "Choice Logic")
	void OnChoiceButtonClicked(const FName& NextNodeId, const FText& ChosenText, const FName RelatedCharacterId = NAME_None, const int32 AffectionToIncrease = 0);
	
	UFUNCTION(BlueprintCallable, Category = "Save Logic")
	void LoadDialogueSpeedData();
	
	UFUNCTION(BlueprintCallable, Category = "Save Logic")
	void SaveDialogueSpeedData() const;

	/* 세이브나 로드를 할때 슬롯에 보여줄 정보를 가져오는 함수 */
	UFUNCTION(BlueprintPure, Category = "Save Logic")
	FLoadSlotUInfo GetSlotUIInfo(const FString& SlotName, const int32 SlotIndex) const;

	/* 실제 슬롯에 데이터를 저장하는 함수 */
	UFUNCTION(BlueprintCallable, Category = "Save Logic")
	bool SaveDialogueDataInSlot(const FString& SlotName, const int32 SlotIndex);

	/* 로드 시 슬롯정보를 저장하고 다이얼로그 맵으로 넘어가는 함수 */
	UFUNCTION(BlueprintCallable, Category = "Save Logic")
	void TravelDialogueMap(const FString& SlotName, const int32 SlotIndex) const;

	//TODO: 미니게임 로드 만들어야함
	// 이미 존재하는 세이브 함수 호출 후 맵 이동 구현하면 됨
	
	UPROPERTY(EditDefaultsOnly)
	FName DialogueID;
	
	/* 대사 문자 출력 속도 */
	UPROPERTY(BlueprintReadWrite)
	EDialogueSpeed DialoguePlaySpeed = EDialogueSpeed::Normal;
	
	/* 스킵 속도 */
	UPROPERTY(BlueprintReadWrite)
	EDialogueSpeed SkipDialogueSpeed = EDialogueSpeed::Slow;
	
	/* 오토 모드시 대화 끝나고 다음 대화로 넘어가는 대기 시간 */
	UPROPERTY(BlueprintReadWrite)
	EDialogueSpeed AutoModePlaySpeed = EDialogueSpeed::Normal; 

	/* 백로그에 저장할 최대 대화 수 */
	UPROPERTY(BlueprintReadOnly)
	int32 BackLogSize = 50;

private:
	void HandleTextDialogue(const UVNDialogueManager* InDialogueManager);
	void HandleChoiceNode(const UVNDialogueManager* InDialogueManager) const;
	void HandleConditionNode(const UVNDialogueManager* InDialogueManager);
	void HandleMiniGameNode(const UVNDialogueManager* InDialogueManager);
	void HandleEndNode() const;

	bool IsConditionSatisfied(const FName& RelatedCharacterId, const int32 RequiredAffection, const TSet<FName>& RequiredNodes) const;

	void AddToBackLog(const ENodeType NodeType, const FText& CharacterName, const FText& LogText);

	void PlayBackGroundMusic(const TSoftObjectPtr<USoundBase>& BGMToPlay) const;
	void PlaySoundEffect(const TSoftObjectPtr<USoundBase>& SFXToPlay) const;

	void StopBackGroundMusic() const;
	void StopSoundEffect() const;

	FText GetCurrentTimeText() const;

	void LoadDataAfterMiniGame();
	void LoadDialogueData();

	UPROPERTY()
	TObjectPtr<UVNDialogueManager> DialogueManager;

	TArray<FBackLogParam> BackLogParams;
	int32 BackLogHeadIndex = 0;

	FText ChapterName;
	FName LastDialogueID;

	/* 미니 게임 왔다갔다 할때 플레이어 정보 저장할 실롯 이름 */
	FString TempSaveSlotName = FString("_TempSlot");

public:
	UFUNCTION(BlueprintPure, Category = "Dialogue Data")
	FORCEINLINE TArray<FBackLogParam> GetBackLogParams() const { return BackLogParams; }
	
};
