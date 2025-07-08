// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

UENUM(BlueprintType)
enum class EVNAbilityType : uint8
{
	None,
	Move,
	Look,
	Jump,

	OpenMenu,
	
	NormalAttack,
	HeavyAttack,
	SpawnProjectile,
	Dodge,

	Hit,
	Death
};

UENUM(BlueprintType)
enum class EAttackNotifyType : uint8
{
	None,
	RightHand,
	LeftHand,
	RightLeg,
	LeftLeg,
	Weapon,
	EndAttack
	
};