// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "DataAsset/Input/DataAsset_InputConfig.h"
#include "VNEnhancedInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class VNMINIGAME_API UVNEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserClass, typename CallbackFunc>
	void BindLocomotionInputAction(const UDataAsset_InputConfig* InputConfigDataAsset, UserClass* ContextObject, const EVNAbilityType InputType, CallbackFunc Callback);

	template<class UserObject, typename CallbackFunc>
	void BindAbilityInputAction(const UDataAsset_InputConfig* InInputConfig, UserObject* ContextObject, CallbackFunc InputPressedFunc);
	
};

template <class UserClass, typename CallbackFunc>
void UVNEnhancedInputComponent::BindLocomotionInputAction(const UDataAsset_InputConfig* InputConfigDataAsset,
	UserClass* ContextObject, const EVNAbilityType InputType, CallbackFunc Callback)
{
	if (!InputConfigDataAsset) return;

	if (const UInputAction* FoundInputAction = InputConfigDataAsset->FindLocomotionInputActionByEnum(InputType))
	{
		BindAction(FoundInputAction, ETriggerEvent::Triggered, ContextObject, Callback);
	}
	
}

template <class UserObject, typename CallbackFunc>
void UVNEnhancedInputComponent::BindAbilityInputAction(const UDataAsset_InputConfig* InInputConfig, UserObject* ContextObject, CallbackFunc InputPressedFunc)
{
	for (const FInputActionParams& AbilityInputActionConfig : InInputConfig->AbilityInputActions)
	{
		if (!AbilityInputActionConfig.IsValid()) continue;
		BindAction(AbilityInputActionConfig.InputAction, ETriggerEvent::Triggered, ContextObject, InputPressedFunc, AbilityInputActionConfig.AbilityType);
	}
}
