// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "AnimIntance/VNCharacterAnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "Character/VNCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

void UVNCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwningCharacter = Cast<AVNCharacterBase>(TryGetPawnOwner());
	if (OwningCharacter)
	{
		OwningMovementComponent = OwningCharacter->GetCharacterMovement();
	}
	
}

void UVNCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (!OwningCharacter) return;

	Velocity = OwningCharacter->GetVelocity();
	GroundSpeed = Velocity.Size2D();
	bHasAcceleration = Velocity.SizeSquared2D() > KINDA_SMALL_NUMBER;
	LocomotionDirection = UKismetAnimationLibrary::CalculateDirection(Velocity, OwningCharacter->GetActorRotation());
	
	bIsFalling = OwningMovementComponent->IsFalling();
	bIsKnockback = OwningCharacter->IsKnockdown();

	bIsStrafing = OwningCharacter->IsStrafing();
	
}
