// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Player/DialoguePlayerController.h"

#include "UI/HUD/VNDialogueHUD.h"

void ADialoguePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (AVNDialogueHUD* HUD = Cast<AVNDialogueHUD>(GetHUD()))
	{
		HUD->InitDialogueOverlay();
	}
	
}
