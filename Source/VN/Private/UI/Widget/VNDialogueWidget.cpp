// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "UI/Widget/VNDialogueWidget.h"

#include "DebugHelper.h"

void UVNDialogueWidget::SetWidgetController(UObject* InWidgetController)
{
	check(InWidgetController);

	WidgetController = InWidgetController;
	OnWidgetControllerSet();
}