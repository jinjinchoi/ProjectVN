// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "VNDialogueHUD.generated.h"

class UMiniGameWidgetController;
class ULoadWidgetController;
class USettingWidgetController;
class UVNDialogueWidget;
class UDialogueWidgetController;

/**
 * 
 */
UCLASS()
class VN_API AVNDialogueHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	UDialogueWidgetController* GetDialogueWidgetController();
	USettingWidgetController* GetSettingWidgetController();
	UMiniGameWidgetController* GetMiniGameWidgetController();

protected:
	virtual void BeginPlay() override;

	/* HUD 활성화 되면 화면에 보여줄 위젯 클래스 */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UVNDialogueWidget> DialogueWidgetClass;

	UPROPERTY()
	TObjectPtr<UVNDialogueWidget> DialogueWidget;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDialogueWidgetController> DialogueWidgetControllerClass;
	
	UPROPERTY()
	TObjectPtr<UDialogueWidgetController> DialogueWidgetController;

	UPROPERTY()
	TObjectPtr<USettingWidgetController> SettingWidgetController;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMiniGameWidgetController> MiniGameWidgetControllerClass;
	
	UPROPERTY()
	TObjectPtr<UMiniGameWidgetController> MiniGameWidgetController;


public:
	void InitDialogueOverlay();
	void InitMiniGameOverlay();
	
	
};
