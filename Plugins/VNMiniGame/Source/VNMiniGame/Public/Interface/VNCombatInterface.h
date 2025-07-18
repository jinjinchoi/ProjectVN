// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VNCombatEnumType.h"
#include "UObject/Interface.h"
#include "VNCombatInterface.generated.h"

class UMotionWarpingComponent;
class UVNStatComponent;
class UVNAbilityComponent;

DECLARE_MULTICAST_DELEGATE(FOnCharacterDied);

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UVNCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class VNMINIGAME_API IVNCombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UVNAbilityComponent* GetAbilityComponent() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UVNStatComponent* GetStatComponent() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetCombatSocketLocation(const EAttackNotifyType AttackType) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDead() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AController* GetOwningController() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UMotionWarpingComponent* GetMotionWarpingComponent() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnCharacterHit(bool InbIsHit);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnCharacterDeath();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsHit() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsKnockback() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void LaunchCombatCharacter(const FVector& LaunchVelocity, const bool bXYOverride, const bool bZOverride, const FVector& AttackLocation, const bool bIsDownAttack);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RequestUseAbility(const EVNAbilityType InAbilityType);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetCharacterCollisionEnabled(const bool InIsEnable);
	
	virtual FOnCharacterDied& GetCharacterDiedDelegate() = 0;
	
};
