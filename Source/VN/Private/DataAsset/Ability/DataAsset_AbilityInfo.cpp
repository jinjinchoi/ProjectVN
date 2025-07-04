// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "DataAsset/Ability/DataAsset_AbilityInfo.h"

FAbilityInfoParams UDataAsset_AbilityInfo::FindAbilityInfoByType(const EVNAbilityType AbilityType) const
{
	if (AbilityType == EVNAbilityType::None) return FAbilityInfoParams();
	
	for (const FAbilityInfoParams& AbilityInfo : AbilityInfoParams)
	{
		if (AbilityInfo.AbilityType == AbilityType)
		{
			return AbilityInfo;
		}
	}

	return FAbilityInfoParams();
}
