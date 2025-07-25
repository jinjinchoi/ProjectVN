// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Component/Combat/VNStatComponent.h"

UVNStatComponent::UVNStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UVNStatComponent::ModifyHealth(const float HealthAmount)
{
	Health = FMath::Clamp(Health + HealthAmount, 0, MaxHealth);

	OnHealthChangedDelegate.Broadcast(Health);
	
	if (Health <= 0)
	{
		OnDeathDelegate.ExecuteIfBound();
	}
}

void UVNStatComponent::RecoveryToMaxHealth()
{
	Health = MaxHealth;
}

void UVNStatComponent::ModifyMana(const float ManaAmount)
{
	Mana = FMath::Clamp(Mana + ManaAmount, 0, MaxMana);

	OnManaChangedDelegate.Broadcast(Mana);
	
}


void UVNStatComponent::BeginPlay()
{
	Super::BeginPlay();

	
}
