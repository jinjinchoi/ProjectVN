// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Player/VNPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Component/Input/VNEnhancedInputComponent.h"
#include "DataAsset/Input/DataAsset_InputConfig.h"
#include "GameFramework/Character.h"
#include "Interface/VNCombatInterface.h"
#include "Interface/VNMiniGameControlInterface.h"
#include "UI/HUD/VNDialogueHUD.h"
#include "UI/WidgetController/MiniGameWidgetController.h"
#include "GameFramework/GameModeBase.h"

AVNPlayerController::AVNPlayerController()
{
	PlayerTeamId = FGenericTeamId(0);
}

FGenericTeamId AVNPlayerController::GetGenericTeamId() const
{
	return PlayerTeamId;
}

void AVNPlayerController::StartDialogue(const FName& NodeId)
{
	if (AVNDialogueHUD* HUD = GetHUD<AVNDialogueHUD>())
	{
		UMiniGameWidgetController* WidgetController = HUD->GetMiniGameWidgetController();
		WidgetController->DialogueNodeId = NodeId;
		WidgetController->CreateDialogueWidget();
	}
}


void AVNPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(InputConfig);
	
	if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		SubSystem->AddMappingContext(InputConfig->DefaultMappingContext, 0);
	}

	const IVNMiniGameControlInterface* GameModeInterface = Cast<IVNMiniGameControlInterface>(GetWorld()->GetAuthGameMode());
	if (!GameModeInterface)
	{
		return;
	}

	switch (GameModeInterface->GetMiniGameType())
	{
	case EMiniGameClearType::None:
		break;

	case EMiniGameClearType::EnemyDefeat:
		SwitchToTopDownInputMode();
		break;

	case EMiniGameClearType::LoadCrossing:
		SwitchToThirdPersonInputMode();
		break;

	case EMiniGameClearType::DistanceReached:
		SwitchToThirdPersonInputMode();
		break;

	default:
		break;
	}
	
}

void AVNPlayerController::SwitchToTopDownInputMode()
{
	if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		SubSystem->AddMappingContext(InputConfig->CombatMappingContext, 1);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
	
}

void AVNPlayerController::SwitchToThirdPersonInputMode()
{
	if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		SubSystem->AddMappingContext(InputConfig->AdventureMappingContext, 1);
	}

	const FInputModeGameOnly InputModeData;
	SetInputMode(InputModeData);
	
}


void AVNPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (AVNDialogueHUD* HUD = Cast<AVNDialogueHUD>(GetHUD()))
	{
		HUD->InitMiniGameOverlay();
	}
	
}

void AVNPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UVNEnhancedInputComponent* VNInputComponent = CastChecked<UVNEnhancedInputComponent>(InputComponent);
	VNInputComponent->BindLocomotionInputAction(InputConfig, this, EVNAbilityType::Move, &ThisClass::Move);
	VNInputComponent->BindLocomotionInputAction(InputConfig, this, EVNAbilityType::Look, &ThisClass::Look);
	VNInputComponent->BindLocomotionInputAction(InputConfig, this, EVNAbilityType::Jump, &ThisClass::Jump);
	VNInputComponent->BindLocomotionInputAction(InputConfig, this, EVNAbilityType::OpenMenu, &ThisClass::OpenMenu);

	VNInputComponent->BindAbilityInputAction(InputConfig, this, &ThisClass::AbilityInputPressed);
	
}


void AVNPlayerController::Move(const FInputActionValue& InputActionValue)
{
	if (IVNCombatInterface::Execute_IsHit(GetPawn()) || IVNCombatInterface::Execute_IsKnockback(GetPawn()) || IVNCombatInterface::Execute_IsDead(GetPawn()))
	{
		return;
	}
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection =  FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
	
}

void AVNPlayerController::Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (LookAxisVector.X != 0.f)
	{
		AddYawInput(LookAxisVector.X);
	}

	if (LookAxisVector.Y != 0.f)
	{
		AddPitchInput(LookAxisVector.Y);
	}
}

void AVNPlayerController::Jump()
{
	if (ACharacter* OwningCharacter = Cast<ACharacter>(GetPawn()))
	{
		OwningCharacter->Jump();
	}
}

void AVNPlayerController::OpenMenu()
{
	check(MenuWidgetClass);

	if (!MenuWidget)
	{
		MenuWidget = CreateWidget<UUserWidget>(this, MenuWidgetClass);
	}
	
	const FInputModeUIOnly InputMode;
	SetInputMode(InputMode);
	MenuWidget->AddToViewport();
}

void AVNPlayerController::AbilityInputPressed(const EVNAbilityType AbilityType)
{
	if (GetPawn()->Implements<UVNCombatInterface>())
	{
		IVNCombatInterface::Execute_RequestUseAbility(GetPawn(), AbilityType);
	}
}


void AVNPlayerController::CreateWinScreen()
{
	check(WinScreenWidgetClass);

	if (!WinScreenWidget)
	{
		WinScreenWidget = CreateWidget<UUserWidget>(this, WinScreenWidgetClass);
	}

	const FInputModeUIOnly InputMode;
	SetInputMode(InputMode);	
	WinScreenWidget->AddToViewport();
	
}

void AVNPlayerController::CreateLoseScreen()
{
	check(LoseScreenWidgetClass);

	if (!LoseScreenWidget)
	{
		LoseScreenWidget = CreateWidget<UUserWidget>(this, LoseScreenWidgetClass);
	}

	const FInputModeUIOnly InputMode;
	SetInputMode(InputMode);	
	LoseScreenWidget->AddToViewport();
	
}

void AVNPlayerController::CreateMenuScreen()
{
	check(MenuWidgetClass);

	if (!MenuWidget)
	{
		MenuWidget = CreateWidget<UUserWidget>(this, MenuWidgetClass);
	}

	const FInputModeUIOnly InputMode;
	SetInputMode(InputMode);
	MenuWidget->AddToViewport();
	
}
