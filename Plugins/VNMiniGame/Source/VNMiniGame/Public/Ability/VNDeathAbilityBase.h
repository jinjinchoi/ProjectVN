// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Ability/VNAbilityBase.h"
#include "VNDeathAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class VNMINIGAME_API UVNDeathAbilityBase : public UVNAbilityBase
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility_Implementation(UVNAbilityComponent* AbilityComponent) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	TArray<UAnimMontage*> DeathAnimMontages;
	
};
