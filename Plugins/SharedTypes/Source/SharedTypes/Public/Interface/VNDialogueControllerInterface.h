// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VNDialogueControllerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UVNDialogueControllerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SHAREDTYPES_API IVNDialogueControllerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void StartDialogue(const FName& NodeId) = 0;
	
};
