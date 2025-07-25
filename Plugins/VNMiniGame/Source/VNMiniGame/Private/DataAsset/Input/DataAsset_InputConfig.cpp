// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "DataAsset/Input/DataAsset_InputConfig.h"

const UInputAction* UDataAsset_InputConfig::FindLocomotionInputActionByEnum(const EVNAbilityType InputType) const
{
	for (const FInputActionParams& InputConfig : LocomotionInputActions)
	{
		if (InputConfig.IsValid() && InputConfig.AbilityType == InputType)
		{
			return InputConfig.InputAction;
		}
	}

	return nullptr;
}
