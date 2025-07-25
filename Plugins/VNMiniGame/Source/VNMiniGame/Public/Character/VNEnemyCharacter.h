// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/VNCharacterBase.h"
#include "Interface/PoolableActorInterface.h"
#include "Interface/VNEnemyInterface.h"
#include "VNEnemyCharacter.generated.h"

class UWidgetComponent; 

/**
 * 
 */
UCLASS()
class VNMINIGAME_API AVNEnemyCharacter : public AVNCharacterBase, public IVNEnemyInterface, public IPoolableActorInterface
{
	GENERATED_BODY()

public:
	AVNEnemyCharacter();
	
	/* begin Combat Interface */
	virtual void OnCharacterHit_Implementation(bool InbIsHit) override;
	virtual void OnCharacterDeath_Implementation() override;
	virtual void RequestUseAbility_Implementation(const EVNAbilityType InAbilityType) override;
	/* end Combat Interface */

	/* begin Enemy Interface */
	virtual AActor* GetActorFromBlackboard(const FName& KeyName) const override;
	/* end Enemy Interface */

	/* begin Pool Interface */
	virtual void OnActivateFromPool_Implementation() override;
	virtual void OnDeactivateToPool_Implementation() override;
	/* end Pool Interface */

	/* begin Character Base */
	virtual void OnCharacterLaunched() override;
	virtual void OnCharacterGotUp() override;
	/* end Character base */

	void SpawnEnemy(const FTransform& SpawnTransform);

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBarComponent;

	UPROPERTY()
	TObjectPtr<AController> CachedController;
};
