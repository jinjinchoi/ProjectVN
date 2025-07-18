// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Ability/VNAbilityBase.h"
#include "VNSpawnProjectileAbilityBase.generated.h"

class AVNProjectileBase;
/**
 * 
 */
UCLASS()
class VNMINIGAME_API UVNSpawnProjectileAbilityBase : public UVNAbilityBase
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility_Implementation(UVNAbilityComponent* AbilityComponent) override;
	virtual void CancelAbility_Implementation() override;

protected:
	virtual void SpawnProjectile(const FVector& SpawnLocation, const FVector& TargetLocation, AActor* TargetActor = nullptr);
	
	UPROPERTY(EditDefaultsOnly, Category = "Projectile", meta = (MustImplement = "PoolableActorInterface"))
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TObjectPtr<UAnimMontage> ProjectileSpawnMontage;

	FName WarpTargetName = FName("ProjectileTarget");

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	bool bIsHomingProjectile;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile", meta = (EditCondition = "bIsHomingProjectile"))
	bool bRotationFollowsVelocity = true;

	UPROPERTY(EditDefaultsOnly, meta=(ClampMin = "0.01", ClampMax = "3.0"))
	float DamageCoefficient = 1.0f;
	
};
