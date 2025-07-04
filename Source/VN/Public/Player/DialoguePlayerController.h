// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DialoguePlayerController.generated.h"

/**
 * 비주얼 노벨 부분에서 사용할 플레이어 컨트롤러
 */
UCLASS()
class VN_API ADialoguePlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
};
