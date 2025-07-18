// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VNDialogueEnumType.h"
#include "Engine/DataAsset.h"
#include "DA_DialogueInfo.generated.h"


USTRUCT(BlueprintType)
struct FDialogueVisualInfo
{
	GENERATED_BODY()

	/* 캐릭터의 고유 아이디 */
	UPROPERTY(EditDefaultsOnly)
	FName CharacterID = FName();

	/* UI에 보여줄 실제 캐릭터 이름 */
	UPROPERTY(EditDefaultsOnly)
	FText CharacterName = FText();

	/* 캐릭터 감정 Enum과 실제 이미지 파일을 저장하는 Map */
	UPROPERTY(EditDefaultsOnly)
	TMap<EDialogueExpression, UTexture2D*> Expressions;
	
};

USTRUCT(BlueprintType)
struct FMiniGameMapInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	EMiniGameType MiniGameType;

	UPROPERTY(EditDefaultsOnly)
	int32 Level = 1;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> MiniGameSoftMap;
	
	
};

/**
 * enum이나 fname을 실제 값과 매칭 시키는 데이터 에셋
 */
UCLASS()
class VNDIALOGUESYSTEM_API UDA_DialogueInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	FText FindCharacterNameById(const FName& CharacterId) const;
	UTexture2D* FindCharacterTexture(const FName& CharacterId, EDialogueExpression Expression) const;
	TSoftObjectPtr<USoundBase> FindSFXByEnum(const EDialogueSFX SoundEffect) const;
	TSoftObjectPtr<USoundBase> FindBGMByEnum(const EDialogueBGM BGM) const;
	TSoftObjectPtr<UTexture2D> FindBackgroundImageByEnum(const EDialogueBGImage BGImage) const;
	TSoftObjectPtr<UTexture2D> FindCutSceneByEnum(const EDialogueCutScene CutScene) const;
	TSoftObjectPtr<UWorld> FindMiniGameWorld(const EMiniGameType MiniGameType, const int32 GameLevel) const;

protected:
	/* 캐릭터 ID와 실제 이름, 이미지를 매칭하여 저장하는 배열. */
	UPROPERTY(EditDefaultsOnly, Category = "Character Info")
	TArray<FDialogueVisualInfo> DialogueVisuals;

	/* 효과음 Enum 매칭 Map */
	UPROPERTY(EditDefaultsOnly, Category = "Sound Info")
	TMap<EDialogueSFX, TSoftObjectPtr<USoundBase>> DialogueSFX;
	
	/* 배경음악 Enum 매칭 Map */
	UPROPERTY(EditDefaultsOnly, Category = "Sound Info")
	TMap<EDialogueBGM, TSoftObjectPtr<USoundBase>> DialogueBGM;

	/* 배경이미지 매칭 Map */
	UPROPERTY(EditDefaultsOnly, Category = "Image Info")
	TMap<EDialogueBGImage, TSoftObjectPtr<UTexture2D>> DialogueBGImage;

	/* 컷신 이미지 매칭 Map */
	UPROPERTY(EditDefaultsOnly, Category = "Image Info")
	TMap<EDialogueCutScene, TSoftObjectPtr<UTexture2D>> DialogueCutScene;
	
	/* 미니게임 매칭 Map */
	UPROPERTY(EditDefaultsOnly, Category = "Game Info")
	TArray<FMiniGameMapInfo> MiniGameData;

	
};
