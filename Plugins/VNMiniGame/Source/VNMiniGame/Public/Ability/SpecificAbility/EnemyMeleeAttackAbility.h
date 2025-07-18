// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Ability/VNDamageAbilityBase.h"
#include "EnemyMeleeAttackAbility.generated.h"

USTRUCT()
struct FAttackData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* Montage = nullptr;

	UPROPERTY(EditDefaultsOnly)
	bool bIsKnockbackAttack = false;

	/* 몽타주 실행 확률 가중치 */
	UPROPERTY(EditDefaultsOnly)
	float Weight = 1.0f;

	/* 대미지 계수 */
	UPROPERTY(EditDefaultsOnly, meta=(ClampMin = "0.0", ClampMax = "2.0"))
	float Coefficient = 1.0f;
};

/**
 * 
 */
UCLASS()
class VNMINIGAME_API UEnemyMeleeAttackAbility : public UVNDamageAbilityBase
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility_Implementation(UVNAbilityComponent* AbilityComponent) override;
	virtual void OnNotifyReceived(const EAttackNotifyType ReceivedNotify) override;

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TArray<FAttackData> AttackMontageData;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	FName WarpTargetName = FName("FacingTarget");

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	FName BlackboardKeyName = FName("TargetActor");

private:
	/* 가중치를 기준으로 데이터를 선택하는 함수 */
	FAttackData PickRandomAttackByWeight();
	FAttackData CachedAttackData;
	
};
