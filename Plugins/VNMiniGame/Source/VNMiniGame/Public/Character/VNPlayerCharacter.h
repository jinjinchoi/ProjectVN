// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/VNCharacterBase.h"
#include "Interface/VNPlayerInterface.h"
#include "VNPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

/**
 * 
 */
UCLASS()
class VNMINIGAME_API AVNPlayerCharacter : public AVNCharacterBase, public IVNPlayerInterface
{
	GENERATED_BODY()
public:
	AVNPlayerCharacter();

	/** begin Player Interface **/
	virtual void RequestUseAbility_Implementation(const EVNAbilityType InAbilityType) override;
	virtual FVector GetMouseCursorPosition() const override;
	virtual void SetInvincibleState(const bool InIsInvincible) override;
	/** end Player Interface **/

	/* begin Combat Interface */
	virtual void OnCharacterDeath_Implementation() override;
	/* end Combat Interface */

	/** begin Character Class **/
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	/** end Character Class **/

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USpringArmComponent> SpringArm;

	FVector CachedMouseHitLocation ;

	void SwitchToTopDownCamera() const;
	void SwitchToThirdPersonCamera() const;
};
