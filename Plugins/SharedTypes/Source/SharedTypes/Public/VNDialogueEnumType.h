// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

enum class EDialogueType : uint8
{
	None,
	Dialogue,
	Choice,
	Condition,
	MiniGame,
	End
};

UENUM(BlueprintType)
enum class EDialoguePosition : uint8
{
	None,
	Center,
	Left,
	Right
};


UENUM(BlueprintType)
enum class EDialogueExpression : uint8
{
	None,
	Neutral,
	Happy,
	Angry,
	Sad,
	Surprised,
	Embarrassed
};

UENUM(BlueprintType)
enum class EDialogueSFX : uint8
{
	None,
	Stop
};


UENUM(BlueprintType)
enum class EDialogueBGM : uint8
{
	None,
	Stop,
	Test,
	Main
};


UENUM(BlueprintType)
enum class EDialogueBGImage : uint8
{
	None,
	Same,
	Test,
	Street,
	School
};

UENUM(BlueprintType)
enum class EDialogueCutScene : uint8
{
	None,
	Test
};

UENUM(BlueprintType)
enum class EMiniGameType: uint8
{
	None,
	Combat,
	RoadCrossing,
	ObstacleCourse
	
};

UENUM(BlueprintType)
enum class EDialogueSpeed : uint8
{
	Slow,
	Normal,
	Fast
};