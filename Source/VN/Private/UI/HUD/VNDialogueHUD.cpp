// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "UI/HUD/VNDialogueHUD.h"

#include "UI/Widget/VNDialogueWidget.h"
#include "UI/WidgetController/DialogueWidgetController.h"
#include "UI/WidgetController/SettingWidgetController.h"

void AVNDialogueHUD::BeginPlay()
{
	Super::BeginPlay();

	InitOverlay();
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
	if (SettingWidgetController == nullptr)
	{
		SettingWidgetController = NewObject<USettingWidgetController>(this);
		SettingWidgetController->SetPlayerController(GetOwningPlayerController());
	}

	return SettingWidgetController;
	
}

void AVNDialogueHUD::InitOverlay()
{
	check(DialogueWidgetClass && DialogueWidgetClass);

	if (!DialogueWidget)
	{
		DialogueWidget = CreateWidget<UVNDialogueWidget>(GetWorld(), DialogueWidgetClass);
	}

	GetDialogueWidgetController();
	DialogueWidget->SetWidgetController(DialogueWidgetController);
	DialogueWidget->AddToViewport();
	
}
