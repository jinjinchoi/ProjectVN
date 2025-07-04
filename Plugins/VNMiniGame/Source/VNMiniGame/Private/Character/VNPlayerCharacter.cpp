// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Character/VNPlayerCharacter.h"

#include "DebugHelper.h"
#include "Camera/CameraComponent.h"
#include "Component/Combat/VNAbilityComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

AVNPlayerCharacter::AVNPlayerCharacter()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 1600.f;
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->SetUsingAbsoluteRotation(true);
	SpringArm->bDoCollisionTest = false;
	SpringArm->SetRelativeRotation(FRotator(-42.f, 0.f, 0.f));

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("Follow Camera");
	FollowCamera->SetupAttachment(SpringArm);
	FollowCamera->bUsePawnControlRotation = false;
	FollowCamera->FieldOfView = 55.f;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	
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

void AVNPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("Player"));
	
}
