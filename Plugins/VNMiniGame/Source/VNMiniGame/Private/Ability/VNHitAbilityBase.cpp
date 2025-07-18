// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Ability/VNHitAbilityBase.h"

#include "Interface/VNCombatInterface.h"

void UVNHitAbilityBase::ActivateAbility_Implementation(UVNAbilityComponent* AbilityComponent)
{
	Super::ActivateAbility_Implementation(AbilityComponent);

	if (HitMontages.Num() <= 0)
	{
		EndAbility();
		return;
	}

	IVNCombatInterface::Execute_OnCharacterHit(AbilityContext.Avatar, true);

	const int32 RandomIndex = FMath::RandRange(0, HitMontages.Num() - 1);
	PlayingHitMontage = HitMontages[RandomIndex];
	
	PlayMontageAndWaitMontageEnd(PlayingHitMontage, [this]()
	{
		IVNCombatInterface::Execute_OnCharacterHit(AbilityContext.Avatar, false);
	});
	
}

void UVNHitAbilityBase::CancelAbility_Implementation()
{
	const AActor* Owner = AbilityContext.Avatar;
	const USkeletalMeshComponent* Mesh = Owner->FindComponentByClass<USkeletalMeshComponent>();
	UAnimInstance* AnimInstance = Mesh ? Mesh->GetAnimInstance() : nullptr;

	if (!PlayingHitMontage || !AnimInstance) return;

	AnimInstance->Montage_Stop(0.2f, PlayingHitMontage);
	PlayingHitMontage = nullptr;
	
	IVNCombatInterface::Execute_OnCharacterHit(AbilityContext.Avatar, false);
	
	Super::EndAbility_Implementation();
	
}
