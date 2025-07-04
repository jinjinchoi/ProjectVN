// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StreamableManager.h"
#include "VNSoundManager.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class VN_API UVNSoundManager : public UObject
{
	GENERATED_BODY()

public:
	void PlaySFX(const TSoftObjectPtr<USoundBase>& SoundToPlay);
	void PlayBackGroundMusic(const TSoftObjectPtr<USoundBase>& BGMToPlay);
	void StopBackGroundMusic() const;
	void StopSFX() const;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<USoundBase> TitleBGM;

	UPROPERTY(EditDefaultsOnly)
	float FadeInTime = 1.5f;

	UPROPERTY(EditDefaultsOnly)
	float FadeOutTime = 1.5f;

private:
	void FadeOutSound(UAudioComponent* AudioComponent) const;
	
	FStreamableManager StreamableManager;
	FTimerHandle BGMTimerHandle;

	UPROPERTY()
	TObjectPtr<UAudioComponent> BGMComponent = nullptr;

	UPROPERTY()
	TObjectPtr<UAudioComponent> SFXComponent = nullptr;

	
};
