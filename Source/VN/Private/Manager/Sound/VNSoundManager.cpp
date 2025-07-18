// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Manager/Sound/VNSoundManager.h"

#include "DebugHelper.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"


void UVNSoundManager::PlaySFX(const TSoftObjectPtr<USoundBase>& SoundToPlay)
{
	if (SoundToPlay.IsNull())
	{
		DebugHelper::Print(FString::Printf(TEXT("SFX Asset is NULL at : %hs"), __FUNCTION__));
		return;
	}

	TWeakObjectPtr<UVNSoundManager> WeakThis(this);
	StreamableManager.RequestAsyncLoad(SoundToPlay.ToSoftObjectPath(), [WeakThis, SoundToPlay]()
	{
		UVNSoundManager* SoundManager = WeakThis.Get();
		USoundBase* LoadedSound = SoundToPlay.Get();
		UWorld* World = SoundManager->GetWorld();

		if (!SoundManager || !LoadedSound || !World)
		{
			DebugHelper::Print(TEXT("Failed to play SFX: Missing essential resource."), FColor::Red);
			return;
		}
		
		if (SoundManager->SFXComponent && SoundManager->SFXComponent->IsPlaying())
		{
			SoundManager->SFXComponent->Stop();
			SoundManager->SFXComponent = nullptr;
		}

		SoundManager->SFXComponent = UGameplayStatics::SpawnSound2D(World, LoadedSound);
		
	});
	
}

void UVNSoundManager::PlayBackGroundMusic(const TSoftObjectPtr<USoundBase>& BGMToPlay)
{
	
	if (BGMToPlay.IsNull())
	{
		DebugHelper::Print(FString::Printf(TEXT("BGM Asset is NULL at : %hs"), __FUNCTION__), FColor::Yellow);
		return;
	}
	
	StopBackGroundMusic();

	TWeakObjectPtr<UVNSoundManager> WeakThis(this);
	StreamableManager.RequestAsyncLoad(BGMToPlay.ToSoftObjectPath(), [WeakThis, BGMToPlay]()
	{
		UVNSoundManager* SoundManager = WeakThis.Get();
		USoundBase* LoadedSound = BGMToPlay.Get();
		UWorld* World = SoundManager->GetWorld();

		if (!SoundManager || !LoadedSound || !World)
		{
			DebugHelper::Print(TEXT("Failed to play SFX: Missing essential resource."), FColor::Red);
			return;
		}
		
		World->GetTimerManager().ClearTimer(SoundManager->BGMTimerHandle);
		World->GetTimerManager().SetTimer(SoundManager->BGMTimerHandle, [WeakThis, LoadedSound]()
		{
			UVNSoundManager* InnerSoundManager = WeakThis.Get();
			UWorld* World = InnerSoundManager->GetWorld();
			if (!World || !InnerSoundManager) return;

			InnerSoundManager->BGMComponent = UGameplayStatics::SpawnSound2D(World, LoadedSound);
			InnerSoundManager->BGMComponent->FadeIn(InnerSoundManager->FadeInTime, 1);
			
		}, SoundManager->FadeOutTime, false);
		
	});
}

void UVNSoundManager::StopBackGroundMusic() const
{
	FadeOutSound(BGMComponent);
}

void UVNSoundManager::StopSFX() const
{
	FadeOutSound(SFXComponent);
}

void UVNSoundManager::FadeOutSound(UAudioComponent* AudioComponent) const
{
	if (AudioComponent && AudioComponent->IsPlaying())
	{
		AudioComponent->FadeOut(FadeOutTime, 0);
		AudioComponent = nullptr;
	}
}
