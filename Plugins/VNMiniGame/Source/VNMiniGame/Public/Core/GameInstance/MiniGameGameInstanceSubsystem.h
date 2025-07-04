// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MiniGameGameInstanceSubsystem.generated.h"

class UVNObjectPoolManager;
/**
 * 
 */
UCLASS()
class VNMINIGAME_API UMiniGameGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void InitSubsystem();
	void ClearPoolManager() const;
	
	TSubclassOf<UVNObjectPoolManager> ObjectPoolManagerClass;

private:
	UPROPERTY()
	TObjectPtr<UVNObjectPoolManager> ObjectPoolManager;

public:
	FORCEINLINE UVNObjectPoolManager* GetObjectPoolManager() { return ObjectPoolManager; }
	
};
