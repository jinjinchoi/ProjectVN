// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Node/VNDialogueNodeBase.h"
#include "VNMiniGameNode.generated.h"

/**
 * 
 */
UCLASS()
class VNDIALOGUESYSTEM_API UVNMiniGameNode : public UVNDialogueNodeBase
{
	GENERATED_BODY()

public:
	EMiniGameType MiniGameType;
	int32 MiniGameLevel = 1;
	FName MiniGameName;
	FName NodeIdAfterClear;
	
};
