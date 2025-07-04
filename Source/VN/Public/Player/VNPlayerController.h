// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "VNCombatEnumType.h"
#include "GameFramework/PlayerController.h"
#include "VNPlayerController.generated.h"

struct FInputActionValue;
class UDataAsset_InputConfig;

/**
 * 미니 게임에서 사용할 플레이어 컨트롤러
 */
UCLASS()
class VN_API AVNPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AVNPlayerController();

	/* begin IGenericTeamAgentInterface */
	virtual FGenericTeamId GetGenericTeamId() const override;
	/* end IGenericTeamAgentInterface */

	void CreateWinScreen();
	void CreateLoseScreen();
	void CreateMenuScreen();
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;

	/* 미니게임을 클리어 하면 보여줄 위젯 */
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> WinScreenWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> WinScreenWidget;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> LoseScreenWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> LoseScreenWidget;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> MenuWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> MenuWidget;
	

private:
	UPROPERTY(EditDefaultsOnly, Category = "Data Asset")
	TObjectPtr<UDataAsset_InputConfig> InputConfig;

	void Move(const FInputActionValue& InputActionValue);

	void AbilityInputPressed(const EVNAbilityType AbilityType);

	FGenericTeamId PlayerTeamId;
	
};
