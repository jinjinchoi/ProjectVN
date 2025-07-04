// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VNDialogueWidget.generated.h"

/**
 * 
 */
UCLASS()
class VN_API UVNDialogueWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	UObject* GetWidgetController() const { return WidgetController; }

	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnWidgetControllerSet();
	
private:
	UPROPERTY()
	TObjectPtr<UObject> WidgetController;
	
};
