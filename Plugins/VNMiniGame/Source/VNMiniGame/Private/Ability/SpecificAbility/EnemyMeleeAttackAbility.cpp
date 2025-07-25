// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Ability/SpecificAbility/EnemyMeleeAttackAbility.h"
#include "MotionWarpingComponent.h"
#include "Interface/VNCombatInterface.h"
#include "Interface/VNEnemyInterface.h"

void UEnemyMeleeAttackAbility::ActivateAbility_Implementation(UVNAbilityComponent* AbilityComponent)
{
	Super::ActivateAbility_Implementation(AbilityComponent);

	if (AttackMontageData.Num() == 0)
	{
		EndAbility();
		return;
	}

	if (UMotionWarpingComponent* MotionWarpingComponent = IVNCombatInterface::Execute_GetMotionWarpingComponent(AbilityContext.Avatar))
	{
		const IVNEnemyInterface* EnemyInterface = Cast<IVNEnemyInterface>(AbilityContext.Avatar);
		check (EnemyInterface);
		
		if (const AActor* FoundActor = EnemyInterface->GetActorFromBlackboard(BlackboardKeyName))
		{
			MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(WarpTargetName, FoundActor->GetActorLocation());
		}
	}
	
	CachedAttackData = PickRandomAttackByWeight();
	PlayMontageAndWaitMontageEnd(CachedAttackData.Montage, [this]()
	{
		
	});
	
	
}

void UEnemyMeleeAttackAbility::OnNotifyReceived(const EAttackNotifyType ReceivedNotify)
{
	if (CachedAttackData.Montage == nullptr) return;
	
	ApplyDamage(ReceivedNotify, CachedAttackData.Coefficient, CachedAttackData.bIsKnockbackAttack);
	
}

FAttackData UEnemyMeleeAttackAbility::PickRandomAttackByWeight()
{
	float TotalWeight = 0.0f;

	for (const FAttackData& Data : AttackMontageData)
	{
		TotalWeight += Data.Weight;
	}

	const float RandomValue = FMath::RandRange(0.0f, TotalWeight);
	float AccumulatedWeight = 0.0f;

	for (const FAttackData& Data : AttackMontageData)
	{
		AccumulatedWeight += Data.Weight;
		if (RandomValue <= AccumulatedWeight)
		{
			return Data;
		}
		
	}

	return AttackMontageData.Last();
	
}
