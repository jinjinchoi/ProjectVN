// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VNCombatEnumType.h"
#include "DataAsset/Ability/DataAsset_AbilityInfo.h"
#include "UI/WidgetController/VNWidgetControllerBase.h"
#include "MiniGameWidgetController.generated.h"

class UVNDialogueManager;
class UVNAbilityComponent;
class UDataAsset_AbilityInfo;
class UVNStatComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChanged, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilitySet);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueStarted);


/**
 * 
 */
UCLASS()
class VN_API UMiniGameWidgetController : public UVNWidgetControllerBase
{
	GENERATED_BODY()

public:
	/* 체력과 마나 기본 값 가져오는 함수 */
	UFUNCTION(BlueprintCallable)
	void BroadCastInitialValue();
	
	void BindCallbacksToDependencies();

	UFUNCTION(BlueprintCallable)
	void TravelDialogueMap();

	UFUNCTION(BlueprintCallable)
	void RestartMiniGameMap() const;

	UFUNCTION(BlueprintPure)
	bool DoseMiniGameUseSkill() const;

	/* 옵션창이 닫히고 마우스를 보여줘야하는지 알려주는 함수 */
	UFUNCTION(BlueprintPure)
	bool DoseShowMouseCursor() const;

	// 다이얼로그 볼륨에 닿았을 때 대화 가져오고 처리하는 함수
	UFUNCTION(BlueprintCallable)
	void BroadCastDialogueData();
	
	void CreateDialogueWidget() const;
	
	FName DialogueNodeId = NAME_None;

	/* 스킬의 아이콘이나 마나 정보 등을 가져오는 함수 */
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

	UPROPERTY(BlueprintAssignable, Category = "Dialogue")
	FOnDialogueStarted OnDialogueStartedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Dialogue")
	FOnTextDialogueInfoReceived TextDialogueInfoReceived;

	UPROPERTY(BlueprintAssignable, Category = "Dialogue")
	FOnEndNodeReceived EndNodeReceived;
	

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

	void HandleDialogueNode();

	UPROPERTY()
	TObjectPtr<UVNDialogueManager> DialogueManager;

	
	
};
