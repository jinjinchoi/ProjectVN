// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "VNFunctionLibrary.h"

#include "UI/HUD/VNDialogueHUD.h"

USettingWidgetController* UVNFunctionLibrary::GetSettingWidgetController(const UObject* WorldContextObject)
{
	if (!WorldContextObject) return nullptr;

	if (const APlayerController* PlayerController = WorldContextObject->GetWorld()->GetFirstPlayerController())
	{
		if (AVNDialogueHUD* DialogueHUD = PlayerController->GetHUD<AVNDialogueHUD>())
		{
			return DialogueHUD->GetSettingWidgetController();
		}
	}

	return nullptr;
}

UDialogueWidgetController* UVNFunctionLibrary::GetDialogueWidgetController(const UObject* WorldContextObject)
{
	if (!WorldContextObject) return nullptr;

	if (const APlayerController* PlayerController = WorldContextObject->GetWorld()->GetFirstPlayerController())
	{
		if (AVNDialogueHUD* DialogueHUD = PlayerController->GetHUD<AVNDialogueHUD>())
		{
			return DialogueHUD->GetDialogueWidgetController();
		}
	}

	return nullptr;
}

UMiniGameWidgetController* UVNFunctionLibrary::GetMiniGameWidgetController(const UObject* WorldContextObject)
{
	if (!WorldContextObject) return nullptr;

	if (const APlayerController* PlayerController = WorldContextObject->GetWorld()->GetFirstPlayerController())
	{
		if (AVNDialogueHUD* DialogueHUD = PlayerController->GetHUD<AVNDialogueHUD>())
		{
			return DialogueHUD->GetMiniGameWidgetController();
		}
	}

	return nullptr;
}
