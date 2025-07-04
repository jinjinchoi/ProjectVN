// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Ability/VNDeathAbilityBase.h"

#include "Interface/VNCombatInterface.h"

void UVNDeathAbilityBase::ActivateAbility_Implementation(UVNAbilityComponent* AbilityComponent)
{
	Super::ActivateAbility_Implementation(AbilityComponent);

	if (!AbilityContext.Avatar || !AbilityContext.Avatar->Implements<UVNCombatInterface>()) return;
	if (DeathAnimMontages.Num() <= 0)
	{
		EndAbility();
		return;
	}
	

	const int32 RandomIndex = FMath::RandRange(0, DeathAnimMontages.Num() - 1);
	UAnimMontage* DeathMontage = DeathAnimMontages[RandomIndex];
	
	const AActor* Owner = AbilityContext.Avatar;
	const USkeletalMeshComponent* Mesh = Owner->FindComponentByClass<USkeletalMeshComponent>();
	UAnimInstance* AnimInstance = Mesh ? Mesh->GetAnimInstance() : nullptr;

	if (!DeathMontage || !AnimInstance) return;
	AnimInstance->Montage_Play(DeathMontage);

	IVNCombatInterface::Execute_OnCharacterDeath(AbilityContext.Avatar);
	
	
}
