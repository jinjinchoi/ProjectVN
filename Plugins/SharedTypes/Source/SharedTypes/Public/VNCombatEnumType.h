// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

UENUM(BlueprintType)
enum class EVNAbilityType : uint8
{
	None,
	Move,
	Look,
	Jump,
	
	NormalAttack,
	HeavyAttack,
	Dodge
};

UENUM(BlueprintType)
enum class EAttackNotifyType : uint8
{
	None,
	RightHand,
	LeftHand,
	RightLeg,
	LeftLeg,
	
};