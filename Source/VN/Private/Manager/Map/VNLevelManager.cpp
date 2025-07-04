// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Manager/Map/VNLevelManager.h"

#include "DebugHelper.h"
#include "Kismet/GameplayStatics.h"

void UVNLevelManager::TravelDialogueMap()
{
	LoadLevelAsync(DialogueMap);
}

void UVNLevelManager::TravelSpecificMap(const TSoftObjectPtr<UWorld>& LevelToTravel)
{
	LoadLevelAsync(LevelToTravel);
}

void UVNLevelManager::LoadLevelAsync(const TSoftObjectPtr<UWorld>& LevelToTravel)
{
	if (LevelToTravel.IsNull())
	{
		DebugHelper::Print(TEXT("SoftLevelReference is null"), FColor::Yellow);
		return;
	}

	TWeakObjectPtr<UObject> WeakThis(this);
	FSoftObjectPath LevelPath = LevelToTravel.ToSoftObjectPath();

	StreamableManager.RequestAsyncLoad(LevelPath, [WeakThis, LevelToTravel]()
	{
		if (!WeakThis.IsValid())
		{
			DebugHelper::Print(TEXT("LoadLevelAsync aborted: Object no longer valid"), FColor::Yellow);
			return;
		}
		
		const UWorld* LoadedWorld = LevelToTravel.Get();
		if (!LoadedWorld)
		{
			DebugHelper::Print(TEXT("LoadedWorld NOT valid"), FColor::Yellow);
			return;
		}
		
		UGameplayStatics::OpenLevel(WeakThis->GetWorld(), LoadedWorld->GetFName());
	});
}
