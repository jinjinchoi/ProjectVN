// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Ability/VNSpawnProjectileAbilityBase.h"
#include "EnemyProjectileAbility.generated.h"

/**
 * 
 */
UCLASS()
class VNMINIGAME_API UEnemyProjectileAbility : public UVNSpawnProjectileAbilityBase
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility_Implementation(UVNAbilityComponent* AbilityComponent) override;
	virtual void OnNotifyReceived(const EAttackNotifyType ReceivedNotify) override;
	
};
