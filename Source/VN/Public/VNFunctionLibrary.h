// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VNFunctionLibrary.generated.h"

class UMiniGameWidgetController;
class UDialogueWidgetController;
class USettingWidgetController;
/**
 * 
 */
UCLASS()
class VN_API UVNFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "VNFunctionLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static USettingWidgetController* GetSettingWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "VNFunctionLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UDialogueWidgetController* GetDialogueWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "VNFunctionLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UMiniGameWidgetController* GetMiniGameWidgetController(const UObject* WorldContextObject);
	
};
