// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VNDialogueEnumType.h"
#include "UI/WidgetController/VNWidgetControllerBase.h"
#include "DialogueWidgetController.generated.h"

UENUM(BlueprintType)
enum class ENodeType : uint8
{
	Dialogue,
	Choice
};

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
	
};


class UVNDialogueManager;
struct FTextDialogueInfo;

/* 대화 노드 정보 받아오면 호출할 델리게이트 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTextDialogueInfoReceived, const FTextDialogueInfo&, DialogueInfo);
/* 선택지 노드 정보 받아오면 호출할 델리게이트 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTChoiceNodeInfoReceived, const TArray<FChoiceNodeInfo>&, ChoiceNodeInfo);

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


protected:
	UFUNCTION(BlueprintCallable, Category = "Choice Logic")
	void OnChoiceButtonClicked(const FName& NextNodeId, const FText& ChosenText, const FName RelatedCharacterId = NAME_None, const int32 AffectionToIncrease = 0);

	UFUNCTION(BlueprintCallable, Category = "Save Logic")
	void LoadDialogueSpeedData();
	
	UFUNCTION(BlueprintCallable, Category = "Save Logic")
	void SaveDialogueSpeedData() const;

	UFUNCTION(BlueprintPure, Category = "Save Logic")
	FLoadSlotUInfo GetSlotUIInfo(const FString& SlotName, const int32 SlotIndex) const;

	UFUNCTION(BlueprintCallable, Category = "Save Logic")
	bool SaveDialogueDataInSlot(const FString& SlotName, const int32 SlotIndex);
	
	// TODO: 나중에 세이브 파일에서 값 가져와야함. 
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

	bool IsConditionSatisfied(const FName& RelatedCharacterId, const int32 RequiredAffection, const TSet<FName>& RequiredNodes) const;

	void AddToBackLog(const ENodeType NodeType, const FText& CharacterName, const FText& LogText);

	void PlayBackGroundMusic(const TSoftObjectPtr<USoundBase>& BGMToPlay) const;
	void PlaySoundEffect(const TSoftObjectPtr<USoundBase>& SFXToPlay) const;

	void StopBackGroundMusic() const;
	void StopSoundEffect() const;

	FText GetCurrentTimeText() const;

	UPROPERTY()
	TObjectPtr<UVNDialogueManager> DialogueManager;

	TArray<FBackLogParam> BackLogParams;
	int32 BackLogHeadIndex = 0;

	FText ChapterName;
	FName LastDialogueID;

public:
	UFUNCTION(BlueprintPure, Category = "Dialogue Data")
	FORCEINLINE TArray<FBackLogParam> GetBackLogParams() const { return BackLogParams; }
	
};
