// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "UI/Widget/VNMiniGameUserWidget.h"

void UVNMiniGameUserWidget::SetWidgetController(UObject* InWidgetController)
{
	check(InWidgetController);

	WidgetController = InWidgetController;
	OnWidgetControllerSet();
}
