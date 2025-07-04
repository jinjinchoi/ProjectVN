// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Ability/SpecificAbility/PlayerProjectileAbility.h"

#include "MotionWarpingComponent.h"
#include "VNMiniGameFunctionLibrary.h"
#include "Interface/VNCombatInterface.h"
#include "Interface/VNPlayerInterface.h"

void UPlayerProjectileAbility::ActivateAbility_Implementation(UVNAbilityComponent* AbilityComponent)
{
	Super::ActivateAbility_Implementation(AbilityComponent);

	if (UMotionWarpingComponent* MotionWarpingComponent = IVNCombatInterface::Execute_GetMotionWarpingComponent(AbilityContext.Avatar))
	{
		if (const IVNPlayerInterface* PlayerInterface = Cast<IVNPlayerInterface>(AbilityContext.Avatar))
		{
			MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(WarpTargetName, PlayerInterface->GetMouseCursorPosition());
		}
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

void UPlayerProjectileAbility::OnNotifyReceived(const EAttackNotifyType ReceivedNotify)
{
	if (!bIsActive) return;

	const FVector SocketLocation = IVNCombatInterface::Execute_GetCombatSocketLocation(AbilityContext.Avatar, ReceivedNotify);
	if (const IVNPlayerInterface* PlayerInterface = Cast<IVNPlayerInterface>(AbilityContext.Avatar))
	{
		const FVector CursorLocation = PlayerInterface->GetMouseCursorPosition();

		TArray<AActor*> OverlappingActors;
		TArray<AActor*> IgnoreActors;
		IgnoreActors.Add(AbilityContext.Avatar);
		
		UVNMiniGameFunctionLibrary::GetHostileActorsWithinRadius(
			this, AbilityContext.Avatar, OverlappingActors, IgnoreActors, 1500, CursorLocation);
		
		AActor* ClosestActor = UVNMiniGameFunctionLibrary::GetClosestActor(OverlappingActors, CursorLocation);
		
		SpawnProjectile(SocketLocation, CursorLocation, ClosestActor);
	}
	
	
}