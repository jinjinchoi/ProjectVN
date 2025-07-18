// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StreamableManager.h"
#include "VNLevelManager.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class VN_API UVNLevelManager : public UObject
{
	GENERATED_BODY()

public:
	void TravelDialogueMap();
	void TravelSpecificMap(const TSoftObjectPtr<UWorld>& LevelToTravel);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Map")
	TSoftObjectPtr<UWorld> DialogueMap;

private:
	void LoadLevelAsync(const TSoftObjectPtr<UWorld>& LevelToTravel);
	FStreamableManager StreamableManager;
	
	
};
