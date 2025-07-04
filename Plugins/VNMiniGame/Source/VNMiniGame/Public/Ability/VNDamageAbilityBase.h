// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Ability/VNAbilityBase.h"
#include "VNDamageAbilityBase.generated.h"

class UNiagaraSystem;

/**
 * 
 */
UCLASS()
class VNMINIGAME_API UVNDamageAbilityBase : public UVNAbilityBase
{
	GENERATED_BODY()
public:
	virtual void OnNotifyReceived(const EAttackNotifyType ReceivedNotify) override;
	
protected:
	
	void ApplyDamage(const EAttackNotifyType ReceivedNotify, const float DamageCoefficient = 1.f, const bool bIsDownAttack = false) const;
	
	UPROPERTY(EditDefaultsOnly, Category = "User Properties")
	float AttackRadius = 50.f;

	/* 일반적인 공격으로 대상을 밀어낼때의 힘 */
	UPROPERTY(EditDefaultsOnly, Category = "User Properties")
	float KnockbackPower = 100.f;

	/* 다운 어택시 날려버릴 힘 */
	UPROPERTY(EditDefaultsOnly, Category = "User Properties")
	float DownAttackKnockbackPower = 400.f;

	/* 다운 어택시 위로 띄우는 힘 */
	UPROPERTY(EditDefaultsOnly, Category = "User Properties")
	float UpwardBoost = 500.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	TObjectPtr<UNiagaraSystem> HitEffect;

	/* 공격 성공시 획득할 마나의 양 */
	UPROPERTY(EditDefaultsOnly, Category = "User Properties")
	float ManaBoost = 0.f;

	
};
