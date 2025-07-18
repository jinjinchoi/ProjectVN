// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VNWidgetControllerBase.generated.h"

/* 대화 노드 정보 받아오면 호출할 델리게이트 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTextDialogueInfoReceived, const FTextDialogueInfo&, DialogueInfo);
/* 선택지 노드 정보 받아오면 호출할 델리게이트 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTChoiceNodeInfoReceived, const TArray<FChoiceNodeInfo>&, ChoiceNodeInfo);
/* 엔드 노드 받을 시 호출할 델리게이트 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndNodeReceived);


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
