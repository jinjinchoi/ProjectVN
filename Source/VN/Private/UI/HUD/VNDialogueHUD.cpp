// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "UI/HUD/VNDialogueHUD.h"

#include "UI/Widget/VNDialogueWidget.h"
#include "UI/WidgetController/DialogueWidgetController.h"
#include "UI/WidgetController/MiniGameWidgetController.h"
#include "UI/WidgetController/SettingWidgetController.h"

void AVNDialogueHUD::BeginPlay()
{
	Super::BeginPlay();
	
}

UDialogueWidgetController* AVNDialogueHUD::GetDialogueWidgetController()
{
	if (!DialogueWidgetController)
	{
		DialogueWidgetController = NewObject<UDialogueWidgetController>(this, DialogueWidgetControllerClass);
		DialogueWidgetController->SetPlayerController(GetOwningPlayerController());
		DialogueWidgetController->Init();
	}

	return DialogueWidgetController;
	
}

USettingWidgetController* AVNDialogueHUD::GetSettingWidgetController()
{
	if (!SettingWidgetController)
	{
		SettingWidgetController = NewObject<USettingWidgetController>(this);
		SettingWidgetController->SetPlayerController(GetOwningPlayerController());
	}

	return SettingWidgetController;
	
}

UMiniGameWidgetController* AVNDialogueHUD::GetMiniGameWidgetController()
{
	check(MiniGameWidgetControllerClass)
	
	if (!MiniGameWidgetController)
	{
		MiniGameWidgetController = NewObject<UMiniGameWidgetController>(this, MiniGameWidgetControllerClass);
		MiniGameWidgetController->SetPlayerController(GetOwningPlayerController());
		MiniGameWidgetController->BindCallbacksToDependencies();
	}

	return MiniGameWidgetController;
	
	
}

void AVNDialogueHUD::InitDialogueOverlay()
{
	check(DialogueWidgetClass);

	if (!DialogueWidget)
	{
		DialogueWidget = CreateWidget<UVNDialogueWidget>(GetWorld(), DialogueWidgetClass);
	}
	
	DialogueWidget->SetWidgetController(GetDialogueWidgetController());
	DialogueWidget->AddToViewport();
	
}

void AVNDialogueHUD::InitMiniGameOverlay()
{
	check(DialogueWidgetClass);

	if (!DialogueWidget)
	{
		DialogueWidget = CreateWidget<UVNDialogueWidget>(GetWorld(), DialogueWidgetClass);
	}

	DialogueWidget->SetWidgetController(GetMiniGameWidgetController());
	DialogueWidget->AddToViewport();
	
}
