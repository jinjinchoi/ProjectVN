// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Projectile/VNProjectileBase.h"
#include "MultiHitProjectile.generated.h"

/**
 * 
 */
UCLASS()
class VNMINIGAME_API AMultiHitProjectile : public AVNProjectileBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
	/* begin AVNProjectileBase */
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	/* end AVNProjectileBase */

	/* begin IPoolableActorInterface */
	virtual void OnActivateFromPool_Implementation() override;
	virtual void OnDeactivateToPool_Implementation() override;
	/* end IPoolableActorInterface */
	
	void ApplyPeriodicDamage();
	
	UPROPERTY(EditDefaultsOnly, Category = "Projectile Properties | Combat")
	float DamageInterval = 0.5;

private:
	FTimerHandle DamageTimerHandle;
	
	TSet<TWeakObjectPtr<AActor>> OverlappedActors;
	
};
