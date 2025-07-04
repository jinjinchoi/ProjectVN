// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Ability/VNDamageAbilityBase.h"

#include "DebugHelper.h"
#include "VNMiniGameFunctionLibrary.h"
#include "Component/Combat/VNStatComponent.h"
#include "Interface/VNCombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h" 

void UVNDamageAbilityBase::OnNotifyReceived(const EAttackNotifyType ReceivedNotify)
{
	
}

void UVNDamageAbilityBase::ApplyDamage(const EAttackNotifyType ReceivedNotify, const float DamageCoefficient, const bool bIsDownAttack) const
{
	if (ReceivedNotify == EAttackNotifyType::None)
	{
		return;
	}

	if (!AbilityContext.Avatar->Implements<UVNCombatInterface>())
	{
		return;
	}

	const FVector CombatSocketLocation = IVNCombatInterface::Execute_GetCombatSocketLocation(AbilityContext.Avatar, ReceivedNotify);
	
	if (CombatSocketLocation == FVector::ZeroVector)
	{
		return;
	}

	TArray<AActor*> OverlappingActors;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(AbilityContext.Avatar);
	UVNMiniGameFunctionLibrary::GetLiveActorWithinRadius(this, OverlappingActors, ActorsToIgnore, AttackRadius, CombatSocketLocation);

	AController* InstigatorController = IVNCombatInterface::Execute_GetOwningController(AbilityContext.Avatar);
	if (!InstigatorController)
	{
		DebugHelper::Print(TEXT("Critical Error Detected!! InstigatorController Not Valid"), FColor::Red);
		return;
	}
	
	const float BaseDamage = AbilityContext.OwningStatComponent->GetAttackPower() * DamageCoefficient; 
	const FVector AvatarLocation = AbilityContext.Avatar->GetActorLocation();
	
	for (AActor* TargetActor : OverlappingActors)
	{
		if (!IsValid(AbilityContext.Avatar)) break;
		
		if (!IsValid(TargetActor)) continue;

		if (!UVNMiniGameFunctionLibrary::IsTargetPawnHostile(Cast<APawn>(TargetActor), Cast<APawn>(AbilityContext.Avatar)))
		{
			continue;
		}
		
		const float AppliedDamage = UGameplayStatics::ApplyDamage(
			TargetActor,
			BaseDamage,
			InstigatorController,
			AbilityContext.Avatar,
			UDamageType::StaticClass()
		);

		// 대미지 적용 여부 확인
		if (AppliedDamage <= 0) continue;
		
		if (AbilityContext.OwningStatComponent)
		{
			// 공격 성공시 마나 획득
			AbilityContext.OwningStatComponent->ModifyMana(ManaBoost);
			
		}
		
		FVector ToTargetDirection  = (TargetActor->GetActorLocation() - AvatarLocation).GetSafeNormal();
		
		if (HitEffect)
		{
			const FRotator SpawnRotation = ToTargetDirection.Rotation();
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, HitEffect, TargetActor->GetActorLocation(), SpawnRotation);
		}
		
		FVector KnockbackVelocity = FVector::ZeroVector;
		if (bIsDownAttack)
		{
			KnockbackVelocity = ToTargetDirection * DownAttackKnockbackPower + FVector(0, 0, UpwardBoost);
		}
		else
		{
			if (KnockbackPower == 0) continue;
			
			KnockbackVelocity = ToTargetDirection * KnockbackPower + FVector(0, 0, 10);
		}
		IVNCombatInterface::Execute_LaunchCombatCharacter(TargetActor, KnockbackVelocity, true, true, AvatarLocation, bIsDownAttack);
	}
	
}
