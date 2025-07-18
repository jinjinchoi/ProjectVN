// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VNCombatEnumType.h"
#include "Components/ActorComponent.h"
#include "VNAbilityComponent.generated.h"

DECLARE_DELEGATE(FOnAbilityInitialized);

class UVNAbilityBase;
class UVNStatComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VNMINIGAME_API UVNAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UVNAbilityComponent();
	void Initialize(UVNStatComponent* InStatComponent);

	void TryActivateAbilityByType(const EVNAbilityType AbilityType);
	void DeactivateCurrentActiveAbility() const;

	UFUNCTION(BlueprintCallable)
	void ReceiveCombatAbilityNotify(const EAttackNotifyType Notify) const;

	float GetAbilityRequiredCost(const EVNAbilityType AbilityType);

	FOnAbilityInitialized OnAbilityInitializedDelegate;
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TArray<TSubclassOf<UVNAbilityBase>> AbilityClasses;

	UPROPERTY()
	TMap<EVNAbilityType, UVNAbilityBase*> AbilityMap;
	
	UPROPERTY()
	TObjectPtr<UVNStatComponent> StatComponent;

private:
	void InitAbilities();
	
	UPROPERTY()
	TObjectPtr<UVNAbilityBase> CurrentAbility;
};
