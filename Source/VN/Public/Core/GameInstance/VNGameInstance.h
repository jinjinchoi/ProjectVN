// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "VNGameInstance.generated.h"

class UVNSaveManager;
class UVNSoundManager;
class UVNDialogueManager;

/**
 * 
 */
UCLASS()
class VN_API UVNGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

protected:
	virtual void OnStart() override;
	
	UPROPERTY(EditDefaultsOnly, Category="Dialogue")
	TSubclassOf<UVNDialogueManager> DialogueManagerClass;
	
	UPROPERTY()
	TObjectPtr<UVNDialogueManager> DialogueManager;

	UPROPERTY(EditDefaultsOnly, Category="Sound")
	TSubclassOf<UVNSoundManager> SoundManagerClass;

	UPROPERTY()
	TObjectPtr<UVNSoundManager> SoundManager;

	UPROPERTY()
	TObjectPtr<UVNSaveManager> SaveManager;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundMix> SoundMix;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundClass> MasterSoundClass;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundClass> BGMSoundClass;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundClass> SFXSoundClass;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundClass> UISoundClass;

private:
	void CreateManagerClasses();
	void SetUserConfig();
	void SetSoundMix() const;


public:
	FORCEINLINE UVNDialogueManager* GetDialogueManager() const { return DialogueManager; }
	FORCEINLINE UVNSoundManager* GetSoundManager() const { return SoundManager; }
	FORCEINLINE UVNSaveManager* GetSaveManager() const { return SaveManager; }
	
};
