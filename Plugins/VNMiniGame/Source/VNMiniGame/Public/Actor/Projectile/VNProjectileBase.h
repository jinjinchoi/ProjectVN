// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/PoolableActorInterface.h"
#include "VNProjectileBase.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;
class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class VNMINIGAME_API AVNProjectileBase : public AActor, public IPoolableActorInterface
{
	GENERATED_BODY()
	
public:	
	AVNProjectileBase();
	bool InitProjectile(AActor* InOwner, const float DamageCoefficient = 1.f);
	void SetHomingTarget(AActor* Target, const bool bRotationFollowsVelocity = true);
	void LaunchProjectile(const FVector& SpawnLocation, const FVector& TargetLocation);
	/* 발사체 제거하는 함수 */
	void ProjectileRemove();

	virtual void OnActivateFromPool_Implementation() override;
	virtual void OnDeactivateToPool_Implementation() override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	/* 호밍 프로젝타일의 경우 타겟이 사라지면 실행할 함수 */
	void OnTargetDestroyed();
	bool IsTargetHostile(const AActor* OtherTarget) const;
	void ApplyDamageToTarget(AActor* DamagedActor) const;
	void OnHit(const FVector& LocationOverride = FVector::ZeroVector) const;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereCollision;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> NiagaraComponent; 

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
	
	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile Properties | SFX")
	TObjectPtr<USoundBase> LoopingSound;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile Properties | SFX")
	TObjectPtr<USoundBase> ImpactSound;

	/* 타겟과 Overlap 될 때 사용할 Effect */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile Properties | SFX")
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	/* Projectile 사라질 때 사용할 Effect */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile Properties | SFX")
	TObjectPtr<UNiagaraSystem> DestructionEffect;

	/* Projectile 사라질 때 사용할 Sound */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile Properties | SFX")
	TObjectPtr<USoundBase> DestructionSound;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile Properties | Params")		
	float LifeSpan = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile Properties | Params")		
	float InitSpeed = 250.f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile Properties | Homing")
	float HomingAccelerationMagnitude = 5000.f;

	/* 호밍 중 타겟이 사라지면 자동으로 새로운 타겟 설정할지 설정하는 불리언 변수 */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile Properties | Homing", meta=(EditConditionToggle))
	bool bAutoRetarget = true;
	
	UPROPERTY(EditDefaultsOnly, Category = "Projectile Properties | Homing", meta = (EditCondition = "bAutoRetarget"))
	float RetargetRadius = 3000.f;

private:
	UPROPERTY()
	TObjectPtr<AActor> HomingTargetActor;
	
	FTimerHandle ProjectileLifeSpanTimer;
	
	TWeakObjectPtr<AController> CachedInstigatorController;
	TWeakObjectPtr<AActor> CachedDamageCauser;
	uint8 InstigatorTeamID = 255;
	float ProjectileDamage = 0.0f;

};
