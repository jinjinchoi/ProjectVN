// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interface/VNPoolProviderInterface.h"
#include "VNGameInstance.generated.h"

class UVNObjectPoolManager;
class UVNLevelManager;
class UVNSaveManager;
class UVNSoundManager;
class UVNDialogueManager;

/**
 * 
 */
UCLASS()
class VN_API UVNGameInstance : public UGameInstance, public IVNPoolProviderInterface
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	/* 메인 메뉴로 돌아갈때 모든 상황 리셋하는 함수 */
	void ResetGameMode();
	void ResetMiniGameVariables();

	void SetEnemyDefeatPoolManager();
	void SetCrossingPoolManager();
	void ResetPoolManager() const;

	FString CurrentSaveSlotName = FString();
	int32 CurrentSaveSlotIndex = -1;
	
	/* 미니 게임을 하고 있던 중인지 저장하는 변수. true면 돌아가서 노드 복원할 때 NextDialogueId로 복원해야함 */
	bool bIsPlayingMiniGame = false;
	/* 미니 게임 클리어 후 다시 다이얼로그로 돌아갈때 호출할 노드 아이디 */
	FName NextDialogueId = NAME_None;
	
	/* 미니 게임 클리어 여부. 다이얼로그로 돌아올 때 확인하여 true면 MiniGameNodeId 플레이어 스테이트에 저장하고 호감도 증가 */
	bool bIsMiniGameCleared = false;
	/* 현재 진행중인 미니게임이 있으면 미니게임 이름 */
	FName MiniGameName = NAME_None;
	/* 미니 게임 호감도 보상 */
	TMap<FName /* 관련된 Character ID */, int32 /* 호감도 보상 */> AffectionRewardMap;

protected:
	virtual void OnStart() override;

#pragma region ManagerClasses
	
	UPROPERTY(EditDefaultsOnly, Category="Dialogue")
	TSubclassOf<UVNDialogueManager> DialogueManagerClass;
	
	UPROPERTY()
	TObjectPtr<UVNDialogueManager> DialogueManager;

	UPROPERTY(EditDefaultsOnly, Category="Sound | Sound Manager")
	TSubclassOf<UVNSoundManager> SoundManagerClass;

	UPROPERTY()
	TObjectPtr<UVNSoundManager> SoundManager;

	UPROPERTY()
	TObjectPtr<UVNSaveManager> SaveManager;

	UPROPERTY(EditDefaultsOnly,  Category="Map")
	TSubclassOf<UVNLevelManager> LevelManagerClass;

	UPROPERTY()
	TObjectPtr<UVNLevelManager> LevelManager;

	UPROPERTY(EditDefaultsOnly, Category = "Object Pool")
	TSubclassOf<UVNObjectPoolManager> ObjectPoolManagerClass;

	UPROPERTY()
	TObjectPtr<UVNObjectPoolManager> ObjectPoolManager;

#pragma endregion


#pragma region SoundSettings

	UPROPERTY(EditDefaultsOnly, Category = "Sound | Sound Mix")
	TObjectPtr<USoundMix> SoundMix;

	UPROPERTY(EditDefaultsOnly, Category = "Sound | Sound Mix")
	TObjectPtr<USoundClass> MasterSoundClass;

	UPROPERTY(EditDefaultsOnly, Category = "Sound | Sound Mix")
	TObjectPtr<USoundClass> BGMSoundClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sound | Sound Mix")
	TObjectPtr<USoundClass> SFXSoundClass;

	UPROPERTY(EditDefaultsOnly, Category = "Sound | Sound Mix")
	TObjectPtr<USoundClass> UISoundClass;

#pragma endregion 

private:
	void CreateManagerClasses();
	void SetUserConfig();
	void SetSoundMix() const;


public:
	FORCEINLINE UVNDialogueManager* GetDialogueManager() const { return DialogueManager; }
	FORCEINLINE UVNSoundManager* GetSoundManager() const { return SoundManager; }
	FORCEINLINE UVNSaveManager* GetSaveManager() const { return SaveManager; }
	FORCEINLINE UVNLevelManager* GetLevelManager() const { return LevelManager; }
	/* begin IVNPoolProviderInterface */
	virtual UVNObjectPoolManager* GetObjectPoolManager() const override;
	/* end IVNPoolProviderInterface */
	
};
