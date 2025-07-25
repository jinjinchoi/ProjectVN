// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "VNCombatEnumType.h"
#include "GameFramework/PlayerController.h"
#include "Interface/VNDialogueControllerInterface.h"
#include "VNPlayerController.generated.h"

struct FInputActionValue;
class UDataAsset_InputConfig;

/**
 * 미니 게임에서 사용할 플레이어 컨트롤러
 */
UCLASS()
class VN_API AVNPlayerController : public APlayerController, public IGenericTeamAgentInterface, public IVNDialogueControllerInterface
{
	GENERATED_BODY()

public:
	AVNPlayerController();

	/* begin IGenericTeamAgentInterface */
	virtual FGenericTeamId GetGenericTeamId() const override;
	/* end IGenericTeamAgentInterface */

	/* begin IVNDialogueControllerInterface */
	virtual void StartDialogue(const FName& NodeId) override;
	/* end IVNDialogueControllerInterface */

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

	void SwitchToTopDownInputMode();
	void SwitchToThirdPersonInputMode();

	void Move(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);
	void Jump();
	void OpenMenu();
	void AbilityInputPressed(const EVNAbilityType AbilityType);

	FGenericTeamId PlayerTeamId;
	
};
