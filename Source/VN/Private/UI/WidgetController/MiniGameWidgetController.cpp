// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "UI/WidgetController/MiniGameWidgetController.h"

#include "Component/Combat/VNAbilityComponent.h"
#include "Interface/VNCombatInterface.h"
#include "Component/Combat/VNStatComponent.h"
#include "Core/GameInstance/VNGameInstance.h"
#include "Core/GameMode/VNMiniGameMode.h"
#include "DataAsset/Ability/DataAsset_AbilityInfo.h"
#include "Manager/Map/VNLevelManager.h"

void UMiniGameWidgetController::BroadCastInitialValue()
{
	if (GetStatComponent())
	{
		OnMaxHealthChangedDelegate.Broadcast(GetStatComponent()->GetMaxHealth());
		OnHealthChangedDelegate.Broadcast(GetStatComponent()->GetCurrentHealth());
		OnManaChangedDelegate.Broadcast(GetStatComponent()->GetMana());
		OnMaxManaChangedDelegate.Broadcast(GetStatComponent()->GetMaxMana());
	}
	
}

void UMiniGameWidgetController::BindCallbacksToDependencies()
{
	check(PlayerController);

	if (GetStatComponent())
	{
		GetStatComponent()->OnHealthChangedDelegate.AddUniqueDynamic(this, &ThisClass::OnHealthChanged);
		GetStatComponent()->OnManaChangedDelegate.AddUniqueDynamic(this, &ThisClass::OnManaChanged);
	}

	if (GetAbilityComponent())
	{
		GetAbilityComponent()->OnAbilityInitializedDelegate.BindLambda([this]()
		{
			OnAbilitySetDelegate.Broadcast();
		});
	}
	
}

void UMiniGameWidgetController::TravelDialogueMap()
{
	const UVNGameInstance* GameInstance = Cast<UVNGameInstance>(PlayerController->GetGameInstance());
	if (!GameInstance) return;
	GameInstance->ResetMiniGameSubSystem();

	if (UVNLevelManager* LevelManager = GameInstance->GetLevelManager())
	{
		LevelManager->TravelDialogueMap();
	}
}

FAbilityInfoParams UMiniGameWidgetController::GetAbilityInfoParams(const EVNAbilityType AbilityType) const
{
	check(AbilityInfo)

	FAbilityInfoParams FoundAbilityInfo = AbilityInfo->FindAbilityInfoByType(AbilityType);
	if (GetAbilityComponent())
	{
		FoundAbilityInfo.RequiredMana = GetAbilityComponent()->GetAbilityRequiredCost(AbilityType);
	}

	return FoundAbilityInfo;
	
}

UVNStatComponent* UMiniGameWidgetController::GetStatComponent() const
{
	if (PlayerController->GetPawn()->Implements<UVNCombatInterface>())
	{
		return IVNCombatInterface::Execute_GetStatComponent(PlayerController->GetPawn());
	}
	
	return nullptr;
}

UVNAbilityComponent* UMiniGameWidgetController::GetAbilityComponent() const
{
	if (PlayerController->GetPawn()->Implements<UVNCombatInterface>())
	{
		return IVNCombatInterface::Execute_GetAbilityComponent(PlayerController->GetPawn());
	}

	return nullptr;
}

void UMiniGameWidgetController::OnHealthChanged(float NewHealth)
{
	OnHealthChangedDelegate.Broadcast(NewHealth);
}

void UMiniGameWidgetController::OnManaChanged(float NewMana)
{
	OnManaChangedDelegate.Broadcast(NewMana);
}
