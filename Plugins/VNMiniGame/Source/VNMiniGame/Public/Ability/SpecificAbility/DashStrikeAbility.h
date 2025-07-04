// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Ability/VNDamageAbilityBase.h"
#include "DashStrikeAbility.generated.h"

/**
 * 
 */
UCLASS()
class VNMINIGAME_API UDashStrikeAbility : public UVNDamageAbilityBase
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility_Implementation(UVNAbilityComponent* AbilityComponent) override;
	virtual void OnNotifyReceived(const EAttackNotifyType ReceivedNotify) override;
	
protected:
	void StopEnemyAlongPath();
	void ApplyDamageAlongPath();
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> DashStrikeMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	FName WarpTargetName = FName("DashTarget");

	UPROPERTY(EditDefaultsOnly, Category = "Damage Params")
	float DamageCoefficient = 1.f;

	/* Dash 지속시간 */
	UPROPERTY(EditDefaultsOnly, Category = "Dash Params")
	float DashDuration = 0.3f;

	/* 공격 사거리 */
	UPROPERTY(EditDefaultsOnly, Category = "Dash Params")
	float DashRange = 500.f;

	/* 장애물 여부를 판단할 캡슐 트레이스의 Radius */
	UPROPERTY(EditDefaultsOnly, Category = "Dash Params")
	float CapsuleRadius = 44;
	
	/* 장애물 여부를 판단할 캡슐 트레이스의 HalfHeight */
	UPROPERTY(EditDefaultsOnly, Category = "Dash Params")
	float CapsuleHalfHeight = 88;

	/* 캡슐 트레이스로 감지할 오브젝트의 타입 */
	UPROPERTY(EditDefaultsOnly, Category = "Dash Params")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	UPROPERTY(EditDefaultsOnly, Category = "Dash Params")
	bool bIsDebugCapsule = false;


private:
	FVector GetDestinationLocation(const FVector& StartLocation, const FVector& EndLocation) const;
	bool IsValidLocation(const FVector& TraceStart, TArray<FHitResult> HitResults) const;

	FVector DashStartLocation;
	FVector DashEndLocation;
	
	TSet<TWeakObjectPtr<AActor>> HitActors;

	
	
};
