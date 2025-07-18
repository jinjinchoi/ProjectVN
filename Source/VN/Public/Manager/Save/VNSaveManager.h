// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VNDialogueEnumType.h"
#include "VNSaveManager.generated.h"

#define META_SLOT_NAME TEXT("MetaSlot")

struct FSaveGameDataParams
{
	FText ChapterName = FText(); // 챕터명
	FName NodeName = FName(); // 노드 네임
	TSet<FName> SelectedNodeIds = TSet<FName>(); // 선택되었던 노드들
	TMap<FName, int32> CharacterAffectionMap = TMap<FName, int32>(); // 캐릭터 호감도 맵
	FText SavedTime = FText(); // 저장 시간
};


class UVNSaveGame;
/**
 * 기본로직 설명입니다. 게임매니저가 생성되면 Init() 함수에서 MetaSaveGame 클래스를 불러옵니다.
 * 그 후 메타데이터가 필요한 클래스들에서는 GetMetaSaveGame() MetaSaveGame 클래스 정보를 가져가 데이터를 로드합니다.
 * 개별 슬롯의 데이터는 슬롯을 선택할 때 생성되며 이후 CurrentSaveGame 변수에 저장됩니다.
 * 데이터를 저장할 때는 메타데이터는 슬롯네임이랑 인덱스가 고정되어있기 때문에 값만 전달하면 저장이 되며,
 * 슬롯 데이터는 저장할 요소가 고정되어 있기 때문에 슬롯 네임이랑 인덱스만 보내면 저장이 됩니다.
 */
UCLASS()
class VN_API UVNSaveManager : public UObject
{
	GENERATED_BODY()

public:
	void Init();

	/* 메타 데이터 불러오는 함수 */
	void LoadMetaData();

	/* 메타 정보들 저장 */
	void SaveSoundVolume(const float MasterVolume, const float SFXVolume, const float BGMVolume, const float UIVolume);
	void SaveDialogueSpeedParams(const EDialogueSpeed DialoguePlaySpeed, const EDialogueSpeed SkipSpeed, const EDialogueSpeed AutoModeSpeed);
	// TODO: 챕터 저장 구현 해야함.

	/* 슬롯 데이터 저장하는 함수 */
	void SaveGameDataInSlot(const FString& SlotName, const int32 SlotIndex, const FSaveGameDataParams& SaveGameDataParams);
	
	/**
	 * 슬롯 데이터 불러오는 함수.
	 * 저장된 슬롯 정보가 없으면 nullptr 반환하기 때문에 if 체크 필수.
	 */
	UVNSaveGame* LoadGameDataInSlot(const FString& SlotName, const int32 SlotIndex);

private:
	/* 전체적인 데이터(회차 클리어 정보, 업적 등)를 저장하는 세이브 게임 */
	UPROPERTY()
	TObjectPtr<UVNSaveGame> MetaSaveGame;

	

public:
	FORCEINLINE UVNSaveGame* GetMetaSaveGame() const { return MetaSaveGame; }
	
};
