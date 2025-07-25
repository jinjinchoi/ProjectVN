// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VNMiniGameUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class VNMINIGAME_API UVNMiniGameUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	UObject* GetWidgetController() const { return WidgetController; }

	void SetWidgetController(UObject* InWidgetController);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnWidgetControllerSet();
	
private:
	UPROPERTY()
	TObjectPtr<UObject> WidgetController;
	
};
