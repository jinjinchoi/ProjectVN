// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VNCombatEnumType.h"
#include "Context/VNAbilityContext.h"
#include "VNAbilityBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAbilityEnded)



/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class VNMINIGAME_API UVNAbilityBase : public UObject
{
	GENERATED_BODY()

public:
	/* Ability Component에서 처음 어빌리티를 생성할 때 실행시키는 함수로 아바타 정보 등이 담긴 FVNAbilityContext 구성하여 저장함. */
	void InitAbility(const FVNAbilityContext& Context);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Ability")
	void ActivateAbility(UVNAbilityComponent* AbilityComponent);

	UFUNCTION(BlueprintNativeEvent, Category = "Ability")
	void EndAbility();
	
	UFUNCTION(BlueprintNativeEvent, Category = "Ability")
	void CancelAbility();
	
	bool CanActivate() const;
	void ConsumeCost() const;

	/* 단순 몽타주 실행 함수. */
	void PlayMontageSimple(UAnimMontage* Montage) const;
	
	/* 몽타주 블랜딩 아웃 후 콜백을 실행하는 함수. 몽타주 블랜딩 아웃 후 끝나면 EndAbility 함수를 실행함. */
	void PlayMontageAndWaitMontageBlendingOut(UAnimMontage* Montage, TFunction<void()> MontageEndCallback);
	
	/* 몽타주 엔드 후 콜백을 실행하는 함수. 몽타주가 끝나면 EndAbility 함수를 실행함. */
	void PlayMontageAndWaitMontageEnd(UAnimMontage* Montage, TFunction<void()> MontageEndCallback);

	/* Notify Receive 하면 실행할 함수 */
	virtual void OnNotifyReceived(const EAttackNotifyType ReceivedNotify);
	
	TFunction<void()> OnMontageEndCallback;
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	EVNAbilityType AbilityType;
	
	FOnAbilityEnded OnAbilityEndedDelegate;

	bool bIsActive = false;
	
protected:
	FVNAbilityContext AbilityContext;
	
	UFUNCTION()
	void OnMontageBlendingOut_MontageEnd(UAnimMontage* Montage, bool bInterrupted);
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	float RequiredManaPoint = 0.f;

public:
	FORCEINLINE float GetRequiredCost () const { return RequiredManaPoint; }
	
};
