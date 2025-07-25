// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VNPoolProviderInterface.generated.h"

class UVNObjectPoolManager;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UVNPoolProviderInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SHAREDTYPES_API IVNPoolProviderInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual UVNObjectPoolManager* GetObjectPoolManager() const = 0;
	
};
