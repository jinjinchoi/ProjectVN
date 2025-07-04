// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Ability/VNDamageAbilityBase.h"
#include "ComboAttackAbility.generated.h"

/**
 * 
 */
UCLASS()
class VNMINIGAME_API UComboAttackAbility : public UVNDamageAbilityBase
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility_Implementation(UVNAbilityComponent* AbilityComponent) override;
	virtual void EndAbility_Implementation() override;
	virtual void OnNotifyReceived(const EAttackNotifyType ReceivedNotify) override;
	virtual void CancelAbility_Implementation() override;

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TMap<int32, UAnimMontage*> AttackMontageMap;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	float ComboResetTime = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	FName WarpTargetName = FName("FacingTarget");

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	float AutoTargetingRange = 250.f;


private:
	UPROPERTY()
	TObjectPtr<UAnimMontage> CurrenAttackMontage;
	void ResetComboCount();
	int32 CurrentAttackComboCount = 1;
	int32 LastAttackComboCount = 1;
	FTimerHandle ComboTimerHandle;

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
};
