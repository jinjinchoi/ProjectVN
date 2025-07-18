// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Ability/VNAbilityBase.h"
#include "DodgeAbility.generated.h"

/**
 * 
 */
UCLASS()
class VNMINIGAME_API UDodgeAbility : public UVNAbilityBase
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility_Implementation(UVNAbilityComponent* AbilityComponent) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TObjectPtr<UAnimMontage> DodgeMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	FName WarpTargetName = FName("FacingTarget");
	
};
