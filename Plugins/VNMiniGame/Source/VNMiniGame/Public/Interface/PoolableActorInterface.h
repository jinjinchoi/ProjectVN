// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PoolableActorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPoolableActorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class VNMINIGAME_API IPoolableActorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/* Pool에서 오브젝트를 꺼내고 사용하는 함수 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pooling")
	void OnActivateFromPool();

	/* Pool에 오브젝트를 넣고 사용하는 함수 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pooling")
	void OnDeactivateToPool();
	
};
