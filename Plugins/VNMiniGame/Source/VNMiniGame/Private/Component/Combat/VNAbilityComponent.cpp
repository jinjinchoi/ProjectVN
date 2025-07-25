// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Component/Combat/VNAbilityComponent.h"

#include "DebugHelper.h"
#include "Ability/VNAbilityBase.h"

UVNAbilityComponent::UVNAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UVNAbilityComponent::Initialize(UVNStatComponent* InStatComponent)
{
	StatComponent = InStatComponent;
	InitAbilities();
}


void UVNAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UVNAbilityComponent::InitAbilities()
{
	for (const TSubclassOf<UVNAbilityBase>& AbilityClass : AbilityClasses)
	{
		if (!AbilityClass) continue;

		UVNAbilityBase* Ability = NewObject<UVNAbilityBase>(this, AbilityClass);
		
		if (Ability->AbilityType == EVNAbilityType::None)
		{
			FString DebugMsg = FString::Printf(TEXT("you need to set Ability Type at %s"), *Ability->GetName());
			DebugHelper::Print(DebugMsg, FColor::Yellow);
			continue;
		}

		FVNAbilityContext AbilityContext;
		AbilityContext.OwningAbilityComponent = this;
		AbilityContext.Avatar = GetOwner();
		AbilityContext.OwningStatComponent = StatComponent;

		Ability->OnAbilityEndedDelegate.AddWeakLambda(this, [this]()
		{
			CurrentAbility = nullptr;
		});
		
		Ability->InitAbility(AbilityContext);
		AbilityMap.Add(Ability->AbilityType, Ability);
	}

	OnAbilityInitializedDelegate.ExecuteIfBound();
	
}

void UVNAbilityComponent::TryActivateAbilityByType(const EVNAbilityType AbilityType)
{
	if (AbilityType == EVNAbilityType::None) return;
	
	UVNAbilityBase** FoundAbility = AbilityMap.Find(AbilityType);
	if (!FoundAbility) return;
	
	UVNAbilityBase* Ability = *FoundAbility;
	if (!Ability) return;

	if (!Ability->CanActivate()) return;
	
	if (CurrentAbility)
	{
		if (AbilityType < EVNAbilityType::Hit)
		{
			return;
		}
		CurrentAbility->CancelAbility();
	}
	
	CurrentAbility = Ability;
	Ability->ActivateAbility(this);
	Ability->ConsumeCost();
	
}

void UVNAbilityComponent::DeactivateCurrentActiveAbility() const
{
	if (!CurrentAbility || !CurrentAbility->bIsActive) return;

	CurrentAbility->CancelAbility();
	
}

void UVNAbilityComponent::ReceiveCombatAbilityNotify(const EAttackNotifyType Notify) const
{
	if (CurrentAbility)
	{
		if (CurrentAbility->bIsActive)
		{
			CurrentAbility->OnNotifyReceived(Notify);
		}
	}
}

float UVNAbilityComponent::GetAbilityRequiredCost(const EVNAbilityType AbilityType)
{
	if (AbilityType == EVNAbilityType::None) return 0;

	UVNAbilityBase** FoundAbility = AbilityMap.Find(AbilityType);
	if (!FoundAbility) return 0;
	
	const UVNAbilityBase* Ability = *FoundAbility;
	if (!Ability) return 0;

	return Ability->GetRequiredCost();
}



