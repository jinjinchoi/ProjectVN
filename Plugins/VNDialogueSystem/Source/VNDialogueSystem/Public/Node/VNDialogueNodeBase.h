// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VNDialogueEnumType.h"
#include "VNDialogueNodeBase.generated.h"



/**
 * 
 */
UCLASS()
class VNDIALOGUESYSTEM_API UVNDialogueNodeBase : public UObject
{
	GENERATED_BODY()

public:
	FName NodeID;
	EDialogueType NodeType;
	
};
