// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Ability/SpecificAbility/ComboAttackAbility.h"

#include "DebugHelper.h"
#include "MotionWarpingComponent.h"
#include "VNMiniGameFunctionLibrary.h"
#include "Interface/VNCombatInterface.h"
#include "Interface/VNPlayerInterface.h"

void UComboAttackAbility::ActivateAbility_Implementation(UVNAbilityComponent* AbilityComponent)
{
	Super::ActivateAbility_Implementation(AbilityComponent);

	GetWorld()->GetTimerManager().ClearTimer(ComboTimerHandle);

	CurrenAttackMontage = *AttackMontageMap.Find(CurrentAttackComboCount);
	if (!CurrenAttackMontage)
	{
		EndAbility();
		return;
	}
	
	// 마우스에서 가장 가까운 액터로 모션워핑. 적이 없으면 마우스 위치로 모션워핑
	if (UMotionWarpingComponent* MotionWarpingComponent = IVNCombatInterface::Execute_GetMotionWarpingComponent(
		AbilityContext.Avatar))
	{
		const IVNPlayerInterface* PlayerInterface = Cast<IVNPlayerInterface>(AbilityContext.Avatar);
		check(PlayerInterface);
		const FVector CursorPosition = PlayerInterface->GetMouseCursorPosition();

		TArray<AActor*> OverlappingActors;
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(AbilityContext.Avatar);
		UVNMiniGameFunctionLibrary::GetLiveActorWithinRadius(this, OverlappingActors, ActorsToIgnore,
		                                                     AutoTargetingRange, CursorPosition);

		const FVector ClosestActorLocation = UVNMiniGameFunctionLibrary::GetClosestActorLocation(OverlappingActors, CursorPosition);

		if (ClosestActorLocation != FVector::ZeroVector)
		{
			MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(WarpTargetName, ClosestActorLocation);
		}
		else
		{
			MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(WarpTargetName, CursorPosition);
		}
	}
	
	// 콤보카운트 저장
	LastAttackComboCount = CurrentAttackComboCount;
	
	const AActor* Owner = AbilityContext.Avatar;
	const USkeletalMeshComponent* Mesh = Owner->FindComponentByClass<USkeletalMeshComponent>();
	UAnimInstance* AnimInstance = Mesh ? Mesh->GetAnimInstance() : nullptr;

	if (!CurrenAttackMontage || !AnimInstance) return;
	AnimInstance->Montage_Play(CurrenAttackMontage);
	
	AnimInstance->OnMontageBlendingOut.AddUniqueDynamic(this, &ThisClass::OnAttackMontageEnded);
	
	if (AttackMontageMap.Num() == CurrentAttackComboCount)
	{
		ResetComboCount();
	}
	else
	{
		CurrentAttackComboCount++;
	}
}

void UComboAttackAbility::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		CurrenAttackMontage = nullptr;
		EndAbility();
	}
}


void UComboAttackAbility::EndAbility_Implementation()
{
	Super::EndAbility_Implementation();

	GetWorld()->GetTimerManager().SetTimer(
		ComboTimerHandle,
		this,
		&ThisClass::ResetComboCount,
		ComboResetTime,
		false
	);
}

void UComboAttackAbility::OnNotifyReceived(const EAttackNotifyType ReceivedNotify)
{
	if (ReceivedNotify < EAttackNotifyType::EndAttack)
	{
		const bool bIsLastAttack = AttackMontageMap.Num() == LastAttackComboCount;
		const float DamageCoefficient = 1.0f + (LastAttackComboCount - 1) * 0.1f;
		ApplyDamage(ReceivedNotify, DamageCoefficient, bIsLastAttack);
	}
	else
	{
		EndAbility();
	}
	
}

void UComboAttackAbility::CancelAbility_Implementation()
{
	const AActor* Owner = AbilityContext.Avatar;
	const USkeletalMeshComponent* Mesh = Owner->FindComponentByClass<USkeletalMeshComponent>();
	UAnimInstance* AnimInstance = Mesh ? Mesh->GetAnimInstance() : nullptr;

	if (!CurrenAttackMontage || !AnimInstance) return;

	if (UMotionWarpingComponent* MotionWarpingComponent = IVNCombatInterface::Execute_GetMotionWarpingComponent(AbilityContext.Avatar))
	{
		MotionWarpingComponent->RemoveWarpTarget(WarpTargetName);
	}
	
	AnimInstance->Montage_Stop(0.3f, CurrenAttackMontage);
	CurrenAttackMontage = nullptr;
	CurrentAttackComboCount = 1;

	Super::EndAbility_Implementation();
	
}


void UComboAttackAbility::ResetComboCount()
{
	CurrentAttackComboCount = 1;
}


