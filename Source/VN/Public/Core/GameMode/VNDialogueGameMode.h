// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/GameMode/VNGameModeBase.h"
#include "VNDialogueGameMode.generated.h"

/**
 * 
 */
UCLASS()
class VN_API AVNDialogueGameMode : public AVNGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};
