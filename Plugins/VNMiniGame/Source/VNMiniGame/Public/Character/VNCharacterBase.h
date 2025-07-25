// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/VNCombatInterface.h"
#include "VNCharacterBase.generated.h"

class UVNAbilityComponent;
class UVNStatComponent;
class UMotionWarpingComponent;

UCLASS()
class VNMINIGAME_API AVNCharacterBase : public ACharacter, public IVNCombatInterface
{
	GENERATED_BODY()

public:
	AVNCharacterBase();
	
	/** begin CombatInterface  **/
	virtual UVNAbilityComponent* GetAbilityComponent_Implementation() const override;
	virtual UVNStatComponent* GetStatComponent_Implementation() const override;
	virtual FVector GetCombatSocketLocation_Implementation(const EAttackNotifyType AttackType) const override;
	virtual bool IsDead_Implementation() const override;
	virtual AController* GetOwningController_Implementation() const override;
	virtual UMotionWarpingComponent* GetMotionWarpingComponent_Implementation() const override;
	virtual void OnCharacterHit_Implementation(bool InbIsHit) override;
	virtual void OnCharacterDeath_Implementation() override;
	virtual bool IsHit_Implementation() const override;
	virtual bool IsKnockback_Implementation() const override;
	virtual void LaunchCombatCharacter_Implementation(const FVector& LaunchVelocity, const bool bXYOverride, const bool bZOverride, const FVector& AttackLocation, const bool bIsDownAttack) override;
	virtual void SetCharacterCollisionEnabled_Implementation(const bool InIsEnable) override;
	virtual FOnCharacterDied& GetCharacterDiedDelegate() override;
	/** end CombatInterface  **/

	/* begin Character Interface */
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Landed(const FHitResult& Hit) override;
	/* end Character Interface */


protected:
	/* begin Actor Interface */
	virtual void BeginPlay() override;
	/* end Actor Interface */
	
	virtual void ReviveCharacter();
	

#pragma region Components
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component")
	TObjectPtr<UVNStatComponent> StatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component")
	TObjectPtr<UVNAbilityComponent> AbilityComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component")
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component")
	TObjectPtr<UStaticMeshComponent> WeaponMeshComponent;

#pragma endregion

	/* 피격시 호출할 사운드 */
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float DeathWaiteDuration = 8.f;
	
	bool bIsHit = false;
	bool bIsDead = false;
	bool bIsInvincible = false;
	bool bIsKnockdown = false;
	bool bIsStrafing = false;
	/* 캐릭터가 죽으면 호출할 델리게이트 */
	FOnCharacterDied CharacterDiedDelegate;


#pragma region CombatSocketNames
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	FName RightHandSocketName = FName("RightHandSocket");

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	FName LeftHandSocketName = FName("LeftHandSocket");

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	FName RightLegSocketName = FName("RightLegSocket");

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	FName LeftLegSocketName = FName("LeftLegSocket");

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	FName WeaponSocketName = FName("WeaponSocket");

#pragma endregion

#pragma region KnockBack
	
	virtual void OnCharacterLaunched();
	virtual void OnCharacterGotUp();

	/* 넉백이 가능한 캐릭터인지 설정 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
	bool bCanKnockback = true;

	/* 다운 몽타주 재생이 끝나고 몇 초 더 누워있을지 설정하는 float 변수 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
	float KnockbackAdditionalDownTime = 0.5f;
	
	/* 캐릭터가 Launch 되었을때 재생할 Montage로 꼭 넘어지는 Montage일 필요는 없음. 넘어지는 Montage가 아닐 경우 GetUpMontage 설정하면 안됨. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Montage")
	TObjectPtr<UAnimMontage> DownMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Montage")
	TObjectPtr<UAnimMontage> GetUpMontage;

	/* 넉백 중 타격자 바라보게 설정하는 타이머 핸들*/
	FTimerHandle RotateTimerHandle;
	/* 타격자의 방향을 저장하는 Rotator */
	FRotator FacingTargetRotation;
	/* 보간 속도 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Montage")
	float FacingAttackerRotationInterpSpeed = 10.0f;

#pragma endregion 

private:

#pragma region GetUp
	
	/* 캐릭터가 날라가는 동안 해당 방향 바라보게 Rotate하면서 보간하는 함수 */
	void InterpRotateTick();
	
	void StandUp();
	FTimerHandle StandUpTimer;

#pragma endregion 

	UFUNCTION()
	void OnMontageEnd(class UAnimMontage* Montage, bool bInterrupted);
	
	UPROPERTY()
	TObjectPtr<UAnimInstance> OwningAnimInstance;


public:
	FORCEINLINE bool IsKnockdown() const { return bIsKnockdown; }
	FORCEINLINE bool IsStrafing() const { return bIsStrafing; }

	UFUNCTION(BlueprintCallable)
	void SetIsStrafing(const bool InIsStrafing);

};
