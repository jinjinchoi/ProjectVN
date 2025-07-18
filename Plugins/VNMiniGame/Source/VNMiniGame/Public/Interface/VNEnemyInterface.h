// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VNEnemyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UVNEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class VNMINIGAME_API IVNEnemyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual AActor* GetActorFromBlackboard(const FName& KeyName) const = 0;
	
};
