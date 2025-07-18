// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VNCombatEnumType.h"
#include "Engine/DataAsset.h"
#include "DataAsset_InputConfig.generated.h"

class UInputMappingContext;
class UInputAction;

USTRUCT(BlueprintType)
struct FInputActionParams
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	EVNAbilityType AbilityType = EVNAbilityType::None;

	bool IsValid () const
	{
		return InputAction && AbilityType != EVNAbilityType::None;
	}
	
};

/**
 * 
 */
UCLASS()
class VNMINIGAME_API UDataAsset_InputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	const UInputAction* FindLocomotionInputActionByEnum(const EVNAbilityType InputType) const;
	
	UPROPERTY(EditDefaultsOnly, Category="Input Mapping Context")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category="Input Mapping Context")
	TObjectPtr<UInputMappingContext> CombatMappingContext;

	UPROPERTY(EditDefaultsOnly, Category="Input Mapping Context")
	TObjectPtr<UInputMappingContext> AdventureMappingContext;

	UPROPERTY(EditDefaultsOnly, meta = (TitleProperty = "AbilityType"))
	TArray<FInputActionParams> LocomotionInputActions;

	UPROPERTY(EditDefaultsOnly, meta = (TitleProperty = "AbilityType"))
	TArray<FInputActionParams> AbilityInputActions;
	
	
};
