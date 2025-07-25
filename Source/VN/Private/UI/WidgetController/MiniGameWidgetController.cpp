// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "UI/WidgetController/MiniGameWidgetController.h"

#include "DebugHelper.h"
#include "VNDialogueManager.h"
#include "Component/Combat/VNAbilityComponent.h"
#include "Interface/VNCombatInterface.h"
#include "Component/Combat/VNStatComponent.h"
#include "Core/GameInstance/VNGameInstance.h"
#include "DataAsset/Ability/DataAsset_AbilityInfo.h"
#include "Interface/VNMiniGameControlInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/Map/VNLevelManager.h"
#include "GameFramework/GameModeBase.h"

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
	GameInstance->ResetPoolManager();

	if (UVNLevelManager* LevelManager = GameInstance->GetLevelManager())
	{
		LevelManager->TravelDialogueMap();
	}
}

void UMiniGameWidgetController::RestartMiniGameMap() const
{
	const UVNGameInstance* GameInstance = Cast<UVNGameInstance>(PlayerController->GetGameInstance());
	if (!GameInstance) return;
	GameInstance->ResetPoolManager();
	
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

bool UMiniGameWidgetController::DoseMiniGameUseSkill() const
{
	IVNMiniGameControlInterface* GameModeInterface = Cast<IVNMiniGameControlInterface>(GetWorld()->GetAuthGameMode());
	if (!GameModeInterface) return false;
	
	if (GameModeInterface->GetMiniGameType() == EMiniGameClearType::EnemyDefeat)
	{
		return true;
	}

	return false;
	
}

bool UMiniGameWidgetController::DoseShowMouseCursor() const
{
	const IVNMiniGameControlInterface* GameModeInterface = Cast<IVNMiniGameControlInterface>(GetWorld()->GetAuthGameMode());
	if (!GameModeInterface) return false;

	switch (GameModeInterface->GetMiniGameType())
	{
	case EMiniGameClearType::DistanceReached:
		return false;
	case EMiniGameClearType::LoadCrossing:
		return false;
	case EMiniGameClearType::EnemyDefeat:
		return true;
	default:
		return true;
	}
}

void UMiniGameWidgetController::BroadCastDialogueData()
{
	if (DialogueNodeId == NAME_None) return;
	
	if (!DialogueManager)
	{
		DialogueManager = CastChecked<UVNGameInstance>(GetWorld()->GetGameInstance())->GetDialogueManager();
	}
	
	const EDialogueType DialogueType = DialogueManager->GetDialogueTypeById(DialogueNodeId);

	if (DialogueType == EDialogueType::Dialogue)
	{
		HandleDialogueNode();
	}
	else if (DialogueType == EDialogueType::End)
	{
		EndNodeReceived.Broadcast();
	}
	else
	{
		DebugHelper::Print(TEXT("Invalid DialogueType received"));
	}
}

void UMiniGameWidgetController::HandleDialogueNode()
{
	const FTextDialogueInfo TextDialogueInfo = DialogueManager->GetTextDialogueInfoById(DialogueNodeId);

	if (TextDialogueInfo.bIsValid)
	{
		TextDialogueInfoReceived.Broadcast(TextDialogueInfo);
		DialogueNodeId = TextDialogueInfo.NextNodeId;
	}
}

void UMiniGameWidgetController::CreateDialogueWidget() const
{
	OnDialogueStartedDelegate.Broadcast();
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


