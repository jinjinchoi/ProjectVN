// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Character/VNCharacterBase.h"

#include "DebugHelper.h"
#include "Component/Combat/VNAbilityComponent.h"
#include "Component/Combat/VNStatComponent.h"
#include "MotionWarpingComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AVNCharacterBase::AVNCharacterBase()
{

	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;

	WeaponMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh Component"));
	WeaponMeshComponent->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	WeaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	StatComponent = CreateDefaultSubobject<UVNStatComponent>(TEXT("Stat Component"));
	AbilityComponent = CreateDefaultSubobject<UVNAbilityComponent>(TEXT("Ability Component"));
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("Motion Warping Component"));

}

#pragma region CombatInterfaceFunc

UVNAbilityComponent* AVNCharacterBase::GetAbilityComponent_Implementation() const
{
	return AbilityComponent;
}

UVNStatComponent* AVNCharacterBase::GetStatComponent_Implementation() const
{
	return StatComponent;
}

FVector AVNCharacterBase::GetCombatSocketLocation_Implementation(const EAttackNotifyType AttackType) const
{
	switch (AttackType)
	{
	case EAttackNotifyType::LeftHand:
		return GetMesh()->GetSocketLocation(LeftHandSocketName);

	case EAttackNotifyType::RightHand:
		return GetMesh()->GetSocketLocation(RightHandSocketName);

	case EAttackNotifyType::LeftLeg:
		return GetMesh()->GetSocketLocation(LeftLegSocketName);

	case EAttackNotifyType::RightLeg:
		return GetMesh()->GetSocketLocation(RightLegSocketName);

	case EAttackNotifyType::Weapon:
		{
			if (WeaponMeshComponent)
			{
				return WeaponMeshComponent->GetSocketLocation(WeaponSocketName);
			}
			DebugHelper::Print(TEXT("WeaponMeshComponent is invalid"));
			return FVector::ZeroVector;
		}

	case EAttackNotifyType::None:
		DebugHelper::Print(TEXT("Invalid NotifyType Received"));
		return FVector::ZeroVector;

	default:
		return FVector::ZeroVector;
	}
}

bool AVNCharacterBase::IsDead_Implementation() const
{
	return bIsDead;
}

AController* AVNCharacterBase::GetOwningController_Implementation() const
{
	return GetController();
}

UMotionWarpingComponent* AVNCharacterBase::GetMotionWarpingComponent_Implementation() const
{
	return MotionWarpingComponent;
}

void AVNCharacterBase::OnCharacterHit_Implementation(bool InbIsHit)
{
	bIsHit = InbIsHit;
}

void AVNCharacterBase::OnCharacterDeath_Implementation()
{
	bIsDead = true;
	GetWorldTimerManager().ClearTimer(StandUpTimer);
	
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	CharacterDiedDelegate.Broadcast();
	CharacterDiedDelegate.Clear();
	
	// 플레이어랑 에너미 서로 다르게 행동해야함.
}

bool AVNCharacterBase::IsHit_Implementation() const
{
	return bIsHit;
}

bool AVNCharacterBase::IsKnockback_Implementation() const
{
	return bIsKnockdown;
}

#pragma endregion 

void AVNCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (StatComponent && AbilityComponent)
	{
		AbilityComponent->Initialize(StatComponent);

		StatComponent->OnDeathDelegate.BindLambda([this]()
		{
			bIsDead = true;
		});
	}

	OwningAnimInstance = GetMesh()->GetAnimInstance();
	if (OwningAnimInstance)
	{
		OwningAnimInstance->OnMontageBlendingOut.AddUniqueDynamic(this, &ThisClass::OnMontageEnd);
		OwningAnimInstance->OnMontageEnded.AddUniqueDynamic(this, &ThisClass::OnMontageEnd);
	}
	
}

void AVNCharacterBase::ReviveCharacter()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	
	StatComponent->RecoveryToMaxHealth();
	AbilityComponent->DeactivateCurrentActiveAbility();

	bIsDead = false;
	
}


float AVNCharacterBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                                   class AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsDead || bIsInvincible || !AbilityComponent)
	{
		return 0;
	}

	const float Armor = StatComponent->GetDefencePower();
	const float FinalDamage = DamageAmount - Armor;

	StatComponent->ModifyHealth(-FinalDamage);
	const float CurrentHealth = StatComponent->GetCurrentHealth();
	
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	}
	
	if (CurrentHealth <= 0)
	{
		AbilityComponent->TryActivateAbilityByType(EVNAbilityType::Death);
	}
	else if (!bIsKnockdown)
	{
		AbilityComponent->TryActivateAbilityByType(EVNAbilityType::Hit);
	}
	
	return FinalDamage;
}


void AVNCharacterBase::LaunchCombatCharacter_Implementation(
	const FVector& LaunchVelocity, const bool bXYOverride, const bool bZOverride, const FVector& AttackLocation, const bool bIsDownAttack)
{
	if (bIsDead) return;
	if (!bCanKnockback) return;
	
	LaunchCharacter(LaunchVelocity, bXYOverride, bZOverride);

	if (bIsDownAttack)
	{
		if (AttackLocation != FVector::ZeroVector)
		{
			FacingTargetRotation = UKismetMathLibrary::FindLookAtRotation(
				GetActorLocation(),
				AttackLocation
			);
			GetWorldTimerManager().SetTimer(RotateTimerHandle, this, &AVNCharacterBase::InterpRotateTick, 0.02f, true);
		}
		
		OnCharacterLaunched();
	}

}

FOnCharacterDied& AVNCharacterBase::GetCharacterDiedDelegate()
{
	return CharacterDiedDelegate;
}

void AVNCharacterBase::SetCharacterCollisionEnabled_Implementation(const bool InIsEnable)
{
	if (InIsEnable)
	{
		GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Block);
	}
	else
	{
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	}
}

void AVNCharacterBase::OnCharacterLaunched()
{
	PlayAnimMontage(DownMontage);

	/* Down Montage는 재생되었지만 캐릭터가 Launch 되지 않은 경우에 GetUp 몽타주를 정상 재생 시키기 위해 예방적으로 타이머 설정 */
	if (!GetCharacterMovement()->IsFalling() && GetUpMontage)
	{
		const float DownTime = DownMontage->GetPlayLength();
		GetWorldTimerManager().SetTimer(StandUpTimer, this, &ThisClass::StandUp, DownTime + KnockbackAdditionalDownTime, false);
	}
	
	bIsKnockdown = true;
}

void AVNCharacterBase::InterpRotateTick()
{
	const FRotator CurrentRot = GetActorRotation();
	const FRotator NewRot = FMath::RInterpTo(CurrentRot, FacingTargetRotation, GetWorld()->GetDeltaSeconds(), FacingAttackerRotationInterpSpeed);

	SetActorRotation(NewRot);
	
	if (CurrentRot.Equals(FacingTargetRotation, 1.0f))
	{
		GetWorldTimerManager().ClearTimer(RotateTimerHandle);
	}
}


void AVNCharacterBase::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	
	if (bIsKnockdown && GetUpMontage)
	{
		const float DownTime = DownMontage->GetPlayLength();
		GetWorldTimerManager().SetTimer(StandUpTimer, this, &ThisClass::StandUp, DownTime + KnockbackAdditionalDownTime, false);
	}
	
}

void AVNCharacterBase::StandUp()
{
	if (bIsKnockdown)
	{
		PlayAnimMontage(GetUpMontage);
	}
}

void AVNCharacterBase::OnMontageEnd(class UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == GetUpMontage && !bInterrupted)
	{
		OnCharacterGotUp();
	}
	else if (Montage == DownMontage && !GetUpMontage)
	{
		/* GetUpMontage가 없는 경우 DownMontage 재생이 끝나면 바로 StandUp 상태로 전환 */
		OnCharacterGotUp();
	}
}

void AVNCharacterBase::OnCharacterGotUp()
{
	bIsKnockdown = false;
}


void AVNCharacterBase::SetIsStrafing(const bool InIsStrafing)
{
	bIsStrafing = InIsStrafing;
}