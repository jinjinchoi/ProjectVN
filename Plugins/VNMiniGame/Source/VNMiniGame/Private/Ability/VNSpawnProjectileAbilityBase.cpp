// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Ability/VNSpawnProjectileAbilityBase.h"

#include "DebugHelper.h"
#include "MotionWarpingComponent.h"
#include "VNMiniGameFunctionLibrary.h"
#include "Actor/Projectile/VNProjectileBase.h"
#include "Interface/VNCombatInterface.h"
#include "Manager/Pooling/VNObjectPoolManager.h"

class UMotionWarpingComponent;

void UVNSpawnProjectileAbilityBase::ActivateAbility_Implementation(UVNAbilityComponent* AbilityComponent)
{
	Super::ActivateAbility_Implementation(AbilityComponent);
	
}

void UVNSpawnProjectileAbilityBase::CancelAbility_Implementation()
{
	const AActor* Owner = AbilityContext.Avatar;
	const USkeletalMeshComponent* Mesh = Owner->FindComponentByClass<USkeletalMeshComponent>();
	UAnimInstance* AnimInstance = Mesh ? Mesh->GetAnimInstance() : nullptr;

	if (!ProjectileSpawnMontage || !AnimInstance) return;

	if (UMotionWarpingComponent* MotionWarpingComponent = IVNCombatInterface::Execute_GetMotionWarpingComponent(AbilityContext.Avatar))
	{
		MotionWarpingComponent->RemoveWarpTarget(WarpTargetName);
	}
	
	AnimInstance->Montage_Stop(0.3f, ProjectileSpawnMontage);
	EndAbility();
}

void UVNSpawnProjectileAbilityBase::SpawnProjectile(const FVector& SpawnLocation, const FVector& TargetLocation, AActor* TargetActor)
{
	UVNObjectPoolManager* PoolManager = UVNMiniGameFunctionLibrary::GetPoolManger(this);
	if (!PoolManager)
	{
		DebugHelper::Print(TEXT("Pool Manager is NOT valid"), FColor::Yellow);
		return;
	}

	AActor* PooledObject = PoolManager->GetPooledObject(ProjectileClass);
	if (!PooledObject)
	{
		DebugHelper::Print(TEXT("Object is NOT valid"), FColor::Yellow);
		return;
	}
	
	AVNProjectileBase* Projectile = CastChecked<AVNProjectileBase>(PooledObject);
	if (bIsHomingProjectile && TargetActor)
	{
		Projectile->SetHomingTarget(TargetActor, bRotationFollowsVelocity);
	}
	if (Projectile->InitProjectile(AbilityContext.Avatar, DamageCoefficient))
	{
		Projectile->LaunchProjectile(SpawnLocation, TargetLocation);
	}
	else
	{
		Projectile->ProjectileRemove();
	}
	
}
