// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VNAbilityContext.generated.h"

class UVNAbilityComponent;
class UVNStatComponent;

/**
 * 이 어빌리티를 소유하고 있는 아바타와 아바타의 어빌리티 컴포넌트, 스탯 컴포넌트를 저장하는 구조체
 */
USTRUCT(BlueprintType)
struct FVNAbilityContext
{
	GENERATED_BODY()
	
	UPROPERTY()
	TObjectPtr<AActor> Avatar;

	UPROPERTY()
	TObjectPtr<UVNAbilityComponent> OwningAbilityComponent;

	UPROPERTY()
	TObjectPtr<UVNStatComponent> OwningStatComponent;

	FVNAbilityContext() = default;
	
};