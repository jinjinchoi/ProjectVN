// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Ability/VNAbilityBase.h"

#include "MotionWarpingComponent.h"
#include "Component/Combat/VNStatComponent.h"
#include "Interface/VNCombatInterface.h"


void UVNAbilityBase::InitAbility(const FVNAbilityContext& Context)
{
	AbilityContext = Context;	
}

void UVNAbilityBase::EndAbility_Implementation()
{
	bIsActive = false;
	OnAbilityEndedDelegate.Broadcast();
}

void UVNAbilityBase::CancelAbility_Implementation()
{
	if (!bIsActive)
	{
		return;
	}

	if (AbilityContext.Avatar && AbilityContext.Avatar->Implements<UVNCombatInterface>())
	{
		if (const USkeletalMeshComponent* Mesh = AbilityContext.Avatar->FindComponentByClass<USkeletalMeshComponent>())
		{
			if (UAnimInstance* Anim = Mesh->GetAnimInstance())
			{
				 if (UMotionWarpingComponent* MotionWarpingComponent = IVNCombatInterface::Execute_GetMotionWarpingComponent(AbilityContext.Avatar))
				 {
				 	MotionWarpingComponent->RemoveAllWarpTargets();
				 }
				
				Anim->Montage_Stop(0.5f);
			}
		}
	}
	
	EndAbility();
	
}


bool UVNAbilityBase::CanActivate() const
{
	if (!AbilityContext.OwningStatComponent) return false;

	return RequiredManaPoint <= AbilityContext.OwningStatComponent->GetMana();
	
}

void UVNAbilityBase::ConsumeCost() const
{
	if (!AbilityContext.OwningStatComponent) return;
	if (RequiredManaPoint == 0) return;

	AbilityContext.OwningStatComponent->ModifyMana(-RequiredManaPoint);
	
}

void UVNAbilityBase::PlayMontageSimple(UAnimMontage* Montage) const
{
	const AActor* Owner = AbilityContext.Avatar;
	const USkeletalMeshComponent* Mesh = Owner->FindComponentByClass<USkeletalMeshComponent>();
	UAnimInstance* AnimInstance = Mesh ? Mesh->GetAnimInstance() : nullptr;

	if (!Montage || !AnimInstance) return;
	AnimInstance->Montage_Play(Montage);
}

void UVNAbilityBase::PlayMontageAndWaitMontageBlendingOut(UAnimMontage* Montage, TFunction<void()> MontageEndCallback)
{
	const AActor* Owner = AbilityContext.Avatar;
	const USkeletalMeshComponent* Mesh = Owner->FindComponentByClass<USkeletalMeshComponent>();
	UAnimInstance* AnimInstance = Mesh ? Mesh->GetAnimInstance() : nullptr;

	if (!Montage || !AnimInstance) return;
	AnimInstance->Montage_Play(Montage);

	OnMontageEndCallback = MoveTemp(MontageEndCallback);
	AnimInstance->OnMontageBlendingOut.AddUniqueDynamic(this, &ThisClass::OnMontageBlendingOut_MontageEnd);
}

void UVNAbilityBase::PlayMontageAndWaitMontageEnd(UAnimMontage* Montage, TFunction<void()> MontageEndCallback)
{
	const AActor* Owner = AbilityContext.Avatar;
	const USkeletalMeshComponent* Mesh = Owner->FindComponentByClass<USkeletalMeshComponent>();
	UAnimInstance* AnimInstance = Mesh ? Mesh->GetAnimInstance() : nullptr;

	if (!Montage || !AnimInstance) return;
	AnimInstance->Montage_Play(Montage);

	OnMontageEndCallback = MoveTemp(MontageEndCallback);
	AnimInstance->OnMontageEnded.AddUniqueDynamic(this, &ThisClass::OnMontageBlendingOut_MontageEnd);
}

void UVNAbilityBase::OnNotifyReceived(const EAttackNotifyType ReceivedNotify)
{
}


void UVNAbilityBase::OnMontageBlendingOut_MontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	if (bInterrupted) return;
	
	if (OnMontageEndCallback)
	{
		OnMontageEndCallback();
	}
	if (bIsActive)
	{
		EndAbility();
	}
}


void UVNAbilityBase::ActivateAbility_Implementation(UVNAbilityComponent* AbilityComponent)
{
	bIsActive = true;
	
}

