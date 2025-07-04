// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VNPlayerInterface.generated.h"

class UCapsuleComponent;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UVNPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class VNMINIGAME_API IVNPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual FVector GetMouseCursorPosition() const = 0;
	virtual void SetInvincibleState(const bool InIsInvincible) = 0;
};
