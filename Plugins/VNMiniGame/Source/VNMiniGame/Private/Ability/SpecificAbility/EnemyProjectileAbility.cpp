// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Ability/SpecificAbility/EnemyProjectileAbility.h"

#include "MotionWarpingComponent.h"
#include "VNMiniGameFunctionLibrary.h"
#include "Interface/VNCombatInterface.h"

void UEnemyProjectileAbility::ActivateAbility_Implementation(UVNAbilityComponent* AbilityComponent)
{
	Super::ActivateAbility_Implementation(AbilityComponent);

	if (UMotionWarpingComponent* MotionWarpingComponent = IVNCombatInterface::Execute_GetMotionWarpingComponent(AbilityContext.Avatar))
	{
		const FVector AvatarLocation = AbilityContext.Avatar->GetActorLocation();
		
		TArray<AActor*> OverlappingActors;
		TArray<AActor*> IgnoreActors;
		IgnoreActors.Add(AbilityContext.Avatar);
		
		UVNMiniGameFunctionLibrary::GetHostileActorsWithinRadius(
			this, AbilityContext.Avatar, OverlappingActors, IgnoreActors, 3000, AvatarLocation);

		const FVector ClosestActorLocation = UVNMiniGameFunctionLibrary::GetClosestActorLocation(OverlappingActors, AvatarLocation);

		MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(WarpTargetName, ClosestActorLocation);
		
	}

	if (ProjectileSpawnMontage)
	{
		PlayMontageAndWaitMontageBlendingOut(ProjectileSpawnMontage, [this]()
		{
			EndAbility();
		});
	}
	else
	{
		EndAbility();
	}
	
}

void UEnemyProjectileAbility::OnNotifyReceived(const EAttackNotifyType ReceivedNotify)
{
	if (!bIsActive) return;

	if (ReceivedNotify == EAttackNotifyType::None) return;

	const FVector SocketLocation = IVNCombatInterface::Execute_GetCombatSocketLocation(AbilityContext.Avatar, ReceivedNotify);
	
	TArray<AActor*> OverlappingActors;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(AbilityContext.Avatar);
	
	UVNMiniGameFunctionLibrary::GetHostileActorsWithinRadius(
		this, AbilityContext.Avatar, OverlappingActors, IgnoreActors, 3000, SocketLocation);

	AActor* ClosestActor = UVNMiniGameFunctionLibrary::GetClosestActor(OverlappingActors, SocketLocation);
	const FVector ClosestActorLocation = UVNMiniGameFunctionLibrary::GetClosestActorLocation(OverlappingActors, SocketLocation);
	
	SpawnProjectile(SocketLocation, ClosestActorLocation, ClosestActor);


	
	
}
