// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VNDialogueStructType.h"
#include "Node/VNDialogueNodeBase.h"
#include "VNDialogueChoiceNode.generated.h"

/**
 * 
 */
UCLASS()
class VNDIALOGUESYSTEM_API UVNDialogueChoiceNode : public UVNDialogueNodeBase
{
	GENERATED_BODY()

public:
	TArray<FChoiceNodeInfo> DialogueChoices;
	
};
