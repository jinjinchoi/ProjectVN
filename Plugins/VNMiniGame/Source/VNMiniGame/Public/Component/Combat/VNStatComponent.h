// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VNStatComponent.generated.h"

DECLARE_DELEGATE(FOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, CurrentHealth);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VNMINIGAME_API UVNStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UVNStatComponent();
	void ModifyHealth(const float HealthAmount);
	void RecoveryToMaxHealth();
	void ModifyMana(const float ManaAmount);

	FOnDeath OnDeathDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnManaChangedDelegate;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	float Health = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	float Mana = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	float MaxMana = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	float AttackPower = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	float DefencePower = 5.f;

public:
	FORCEINLINE float GetCurrentHealth () const { return Health; };
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE float GetMana() const { return Mana; }
	FORCEINLINE float GetMaxMana() const { return MaxMana; }
	FORCEINLINE float GetAttackPower() const { return AttackPower; }
	FORCEINLINE float GetDefencePower() const { return DefencePower; }
	
};
