// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VNCombatEnumType.h"
#include "DataAsset/Ability/DataAsset_AbilityInfo.h"
#include "UI/WidgetController/VNWidgetControllerBase.h"
#include "MiniGameWidgetController.generated.h"

class UVNAbilityComponent;
class UDataAsset_AbilityInfo;
class UVNStatComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChanged, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilitySet);


/**
 * 
 */
UCLASS()
class VN_API UMiniGameWidgetController : public UVNWidgetControllerBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void BroadCastInitialValue();
	
	void BindCallbacksToDependencies();

	UFUNCTION(BlueprintCallable)
	void TravelDialogueMap();
	
	UFUNCTION(BlueprintCallable)
	FAbilityInfoParams GetAbilityInfoParams(const EVNAbilityType AbilityType) const;
	
	UPROPERTY(BlueprintAssignable, Category = "Stat")
	FOnAttributeChanged OnHealthChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Stat")
	FOnAttributeChanged OnMaxHealthChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Stat")
	FOnAttributeChanged OnManaChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Stat")
	FOnAttributeChanged OnMaxManaChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Ability")
	FOnAbilitySet OnAbilitySetDelegate;

protected:
	UVNStatComponent* GetStatComponent() const;
	UVNAbilityComponent* GetAbilityComponent() const; 

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TObjectPtr<UDataAsset_AbilityInfo> AbilityInfo;

private:
	UFUNCTION()
	void OnHealthChanged(float NewHealth);

	UFUNCTION()
	void OnManaChanged(float NewMana);

	
	
};
