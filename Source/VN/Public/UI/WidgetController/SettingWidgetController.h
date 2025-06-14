// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/VNWidgetControllerBase.h"
#include "SettingWidgetController.generated.h"

/**
 * 
 */
UCLASS()
class VN_API USettingWidgetController : public UVNWidgetControllerBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void LoadSoundDataIfExist();

	UFUNCTION(BlueprintCallable)
	void SaveSoundData();

protected:
	UPROPERTY(BlueprintReadWrite)
	float MasterVolume = 1.f;

	UPROPERTY(BlueprintReadWrite)
	float BGMVolume = 1.f;
	
	UPROPERTY(BlueprintReadWrite)
	float SFXVolume = 1.f;

	UPROPERTY(BlueprintReadWrite)
	float UIVolume = 1.f;
	
	
};
