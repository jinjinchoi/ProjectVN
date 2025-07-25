// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Ability/SpecificAbility/DodgeAbility.h"

#include "MotionWarpingComponent.h"
#include "Interface/VNCombatInterface.h"
#include "Interface/VNPlayerInterface.h"

void UDodgeAbility::ActivateAbility_Implementation(UVNAbilityComponent* AbilityComponent)
{
	Super::ActivateAbility_Implementation(AbilityComponent);

	if (!DodgeMontage)
	{
		EndAbility();
		return;
	}

	if (IVNPlayerInterface* PlayerInterface = Cast<IVNPlayerInterface>(AbilityContext.Avatar))
	{
		PlayerInterface->SetInvincibleState(true);
		const FVector CursorPosition = PlayerInterface->GetMouseCursorPosition();

		if (UMotionWarpingComponent* MotionWarpingComponent = IVNCombatInterface::Execute_GetMotionWarpingComponent(AbilityContext.Avatar))
		{
			MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(WarpTargetName, CursorPosition);
		}
		
	}
	
	PlayMontageAndWaitMontageEnd(DodgeMontage, [this]()
	{
		if (IVNPlayerInterface* PlayerInterface = Cast<IVNPlayerInterface>(AbilityContext.Avatar))
		{
			PlayerInterface->SetInvincibleState(false);
		}
	});
	
}
