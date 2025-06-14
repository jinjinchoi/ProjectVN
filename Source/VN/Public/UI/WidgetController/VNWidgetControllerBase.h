// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VNWidgetControllerBase.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class VN_API UVNWidgetControllerBase : public UObject
{
	GENERATED_BODY()

public:
	void SetPlayerController(APlayerController* PC);
	virtual void Init(); // 반드시 SetPlayerController() 함수 이후에 사용해야함
	
protected:
	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController;
	
};
