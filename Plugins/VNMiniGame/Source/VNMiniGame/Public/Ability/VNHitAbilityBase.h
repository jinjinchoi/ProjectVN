// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Ability/VNAbilityBase.h"
#include "VNHitAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class VNMINIGAME_API UVNHitAbilityBase : public UVNAbilityBase
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility_Implementation(UVNAbilityComponent* AbilityComponent) override;
	virtual void CancelAbility_Implementation() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	TArray<UAnimMontage*> HitMontages;

private:
	UPROPERTY()
	TObjectPtr<UAnimMontage> PlayingHitMontage;
	
};
