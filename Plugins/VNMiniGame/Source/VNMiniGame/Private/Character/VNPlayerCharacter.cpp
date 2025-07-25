// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Character/VNPlayerCharacter.h"

#include "DebugHelper.h"
#include "Camera/CameraComponent.h"
#include "Component/Combat/VNAbilityComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/VNMiniGameControlInterface.h"

AVNPlayerCharacter::AVNPlayerCharacter()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->SetupAttachment(GetRootComponent());
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>("Follow Camera");
	FollowCamera->SetupAttachment(SpringArm,  USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	InitialLifeSpan = 0.f;
}


void AVNPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("Player"));

	IVNMiniGameControlInterface* GameModeInterface = Cast<IVNMiniGameControlInterface>(GetWorld()->GetAuthGameMode());
	if (!GameModeInterface)
	{
		DebugHelper::Print(TEXT("You need to implement interface to the current game mode"), FColor::Yellow);
	}

	switch (GameModeInterface->GetMiniGameType())
	{
	case EMiniGameClearType::None:
		DebugHelper::Print(TEXT("Invalid Mini Game Type Set"), FColor::Yellow);
		break;

	case EMiniGameClearType::EnemyDefeat:
		SwitchToTopDownCamera();
		break;

	case EMiniGameClearType::LoadCrossing:
		SwitchToThirdPersonCamera();
		break;
		
	case EMiniGameClearType::DistanceReached:
		SwitchToThirdPersonCamera();
		break;

	default:
		break;
	}

	
}

void AVNPlayerCharacter::SwitchToTopDownCamera() const
{
	SpringArm->TargetArmLength = 1600.f;
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->SetUsingAbsoluteRotation(true);
	SpringArm->bDoCollisionTest = false;
	SpringArm->SetRelativeRotation(FRotator(-42.f, 0.f, 0.f));
	
	FollowCamera->FieldOfView = 55.f;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	
}

void AVNPlayerCharacter::SwitchToThirdPersonCamera() const
{
	SpringArm->TargetArmLength = 500.f;
	SpringArm->SocketOffset = FVector(0.f, 0.f, 60.f);
	SpringArm->bUsePawnControlRotation = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.f, 0.f);
}


void AVNPlayerCharacter::RequestUseAbility_Implementation(const EVNAbilityType InAbilityType)
{
	if (!AbilityComponent)
	{
		DebugHelper::Print(TEXT("AbilityComponent Not Valid"), FColor::Red);
		return;
	}

	if (bIsKnockdown || bIsHit)
	{
		return;
	}

	FHitResult HitResult;
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC && PC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
	{
		CachedMouseHitLocation = HitResult.ImpactPoint;
	}
	
	AbilityComponent->TryActivateAbilityByType(InAbilityType);
	
}

FVector AVNPlayerCharacter::GetMouseCursorPosition() const
{
	return CachedMouseHitLocation;
}

void AVNPlayerCharacter::SetInvincibleState(const bool InIsInvincible)
{
	bIsInvincible = InIsInvincible;
}

void AVNPlayerCharacter::OnCharacterDeath_Implementation()
{
	Super::OnCharacterDeath_Implementation();

	SpringArm->bDoCollisionTest = false;
	
	FollowCamera->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	
	TWeakObjectPtr<AActor> WeakThis = this;
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([WeakThis]()
	{
		if (!WeakThis.IsValid()) return;

		if (IVNMiniGameControlInterface* GameModeInterface = Cast<IVNMiniGameControlInterface>(WeakThis->GetWorld()->GetAuthGameMode()))
		{
			GameModeInterface->PlayerLose();
		}
		
	}), DeathWaiteDuration, false);
	
}


/** 플레이어는 넉백 중 무적 상태가 되기 위해 따로 오버라이드 **/
float AVNPlayerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsKnockdown)
	{
		return 0;
	}
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	
}
