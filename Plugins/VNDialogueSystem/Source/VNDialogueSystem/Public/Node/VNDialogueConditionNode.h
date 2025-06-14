// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VNDialogueStructType.h"
#include "Node/VNDialogueNodeBase.h"
#include "VNDialogueConditionNode.generated.h"

/**
 * 
 */
UCLASS()
class VNDIALOGUESYSTEM_API UVNDialogueConditionNode : public UVNDialogueNodeBase
{
	GENERATED_BODY()

public:
	FConditionNodeInfo ConditionNodeInfo = FConditionNodeInfo();
	
};
