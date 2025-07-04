// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Ability/SpecificAbility/DashStrikeAbility.h"

#include "MotionWarpingComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "VNMiniGameFunctionLibrary.h"
#include "Component/Combat/VNAbilityComponent.h"
#include "Component/Combat/VNStatComponent.h"
#include "Interface/VNCombatInterface.h"
#include "Interface/VNPlayerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UDashStrikeAbility::ActivateAbility_Implementation(UVNAbilityComponent* AbilityComponent)
{
	Super::ActivateAbility_Implementation(AbilityComponent);

	HitActors.Reset();

	IVNPlayerInterface* PlayerInterface = Cast<IVNPlayerInterface>(AbilityContext.Avatar);
	check(PlayerInterface)
	
	DashStartLocation = AbilityContext.Avatar->GetActorLocation();
	FVector MouseLocation = PlayerInterface->GetMouseCursorPosition();
	MouseLocation.Z = DashStartLocation.Z;
	
	const FVector Direction = (MouseLocation - DashStartLocation).GetSafeNormal();
	
	const FVector EndLocation = DashStartLocation + Direction * DashRange;
	DashEndLocation = GetDestinationLocation(DashStartLocation, EndLocation);

	if (UMotionWarpingComponent* MotionWarpingComponent = IVNCombatInterface::Execute_GetMotionWarpingComponent(AbilityContext.Avatar))
	{
		MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(WarpTargetName, DashEndLocation);
	}

	IVNCombatInterface::Execute_SetCharacterCollisionEnabled(AbilityContext.Avatar, false);
	
	AbilityContext.Avatar->SetActorRotation(Direction.Rotation());
	PlayerInterface->SetInvincibleState(true);

	PlayMontageSimple(DashStrikeMontage);
	
	
}

void UDashStrikeAbility::OnNotifyReceived(const EAttackNotifyType ReceivedNotify)
{
	if (ReceivedNotify < EAttackNotifyType::EndAttack)
	{
		StopEnemyAlongPath();
		ApplyDamageAlongPath();
	}
	
}

void UDashStrikeAbility::StopEnemyAlongPath()
{
	TArray<FHitResult> HitResults;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(AbilityContext.Avatar);

	const bool bIsHit = GetWorld()->SweepMultiByObjectType(
		HitResults,
		DashStartLocation,
		DashEndLocation,
		FQuat::Identity,
		FCollisionObjectQueryParams(ECC_Pawn),
		FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight),
		CollisionQueryParams
	);
	
	if (!bIsHit) return;

	const APawn* AvatarPawn = Cast<APawn>(AbilityContext.Avatar);
	for (const FHitResult& HitResult : HitResults)
	{
		if (!HitResult.GetActor()) continue;
		
		const APawn* TargetPawn = Cast<APawn>(HitResult.GetActor());
		if (!TargetPawn) continue;
		
		if (!UVNMiniGameFunctionLibrary::IsTargetPawnHostile(AvatarPawn, TargetPawn)) continue;
		if (IVNCombatInterface::Execute_IsDead(TargetPawn)) continue;
		
		HitActors.Add(HitResult.GetActor());
	}
}

void UDashStrikeAbility::ApplyDamageAlongPath()
{
	AController* InstigatorController = IVNCombatInterface::Execute_GetOwningController(AbilityContext.Avatar);
	const float BaseDamage = AbilityContext.OwningStatComponent->GetAttackPower() * DamageCoefficient;
	
	for (TWeakObjectPtr<AActor>& WeakActor : HitActors)
	{
		if (!WeakActor.IsValid()) continue;

		AActor* HitActor = WeakActor.Get();
		if (IVNCombatInterface::Execute_IsDead(HitActor)) continue;
		
		const float AppliedDamage =
			UGameplayStatics::ApplyDamage(HitActor, BaseDamage, InstigatorController, AbilityContext.Avatar, UDamageType::StaticClass());

		if (AppliedDamage <= 0.f) continue;

		if (HitEffect)
		{
			FVector ToTargetDirection =(HitActor->GetActorLocation() - AbilityContext.Avatar->GetActorLocation()).GetSafeNormal();
			const FRotator SpawnRotation = ToTargetDirection.Rotation();
			
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, HitEffect, HitActor->GetActorLocation(), SpawnRotation);
		}
		
		FVector ToTargetDirection  = (DashEndLocation - DashStartLocation).GetSafeNormal();
		FVector KnockbackVelocity = ToTargetDirection * KnockbackPower + FVector(0, 0, 10);
		IVNCombatInterface::Execute_LaunchCombatCharacter(HitActor, KnockbackVelocity, true, true, FVector::ZeroVector, false);
	}

	IVNPlayerInterface* PlayerInterface = Cast<IVNPlayerInterface>(AbilityContext.Avatar);
	PlayerInterface->SetInvincibleState(false);
	IVNCombatInterface::Execute_SetCharacterCollisionEnabled(AbilityContext.Avatar, true);
	
}

FVector UDashStrikeAbility::GetDestinationLocation(const FVector& StartLocation, const FVector& EndLocation) const
{
	const FVector Direction = (EndLocation - StartLocation).GetSafeNormal();
	const float Distance = FVector::Distance(StartLocation, EndLocation);

	const int32 CapsuleCount = FMath::FloorToInt(Distance / CapsuleRadius);

	for (int32 i = CapsuleCount; i >= 0; --i)
	{
		const FVector TraceStartLocation = StartLocation + Direction * (i * CapsuleRadius);
		const FVector TraceEndLocation = TraceStartLocation;

		TArray<FHitResult> HitResults;
		UKismetSystemLibrary::CapsuleTraceMultiForObjects(
			GetWorld(),
			TraceStartLocation,
			TraceEndLocation,
			CapsuleRadius,
			CapsuleHalfHeight,
			ObjectTypes,
			false,
			TArray<AActor*>(),
			bIsDebugCapsule ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
			HitResults,
			true
		);
		
		if (IsValidLocation(TraceStartLocation, HitResults))
		{
			return TraceStartLocation;
		}
	}

	return StartLocation;
	
}

bool UDashStrikeAbility::IsValidLocation(const FVector& TraceStart, TArray<FHitResult> HitResults) const
{
	const FVector TraceEnd = TraceStart - FVector(0, 0, 200);
	
	FHitResult OutHitResult;
	const bool bIsHit = GetWorld()->LineTraceSingleByChannel(
		OutHitResult,
		TraceStart,
		TraceEnd,
		ECC_Visibility
	);

	if (!bIsHit)
	{
		return false;
	}
	
	for (const FHitResult& HitResult : HitResults)
	{
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		if (HitComponent && HitComponent->GetCollisionResponseToChannel(ECC_Pawn) == ECR_Block)
		{
			return false;
		}
	}

	return true;
}


