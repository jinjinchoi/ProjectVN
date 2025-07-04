// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VNCombatEnumType.h"
#include "Engine/DataAsset.h"
#include "DataAsset_AbilityInfo.generated.h"

USTRUCT(BlueprintType)
struct FAbilityInfoParams
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EVNAbilityType AbilityType = EVNAbilityType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> AbilityIcon = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float RequiredMana = 0.f;
	
};

/**
 * 
 */
UCLASS()
class VN_API UDataAsset_AbilityInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Information")
	TArray<FAbilityInfoParams> AbilityInfoParams;

	FAbilityInfoParams FindAbilityInfoByType(const EVNAbilityType AbilityType) const;
	
};
