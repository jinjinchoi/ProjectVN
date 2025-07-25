// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/GameMode/VNGameModeBase.h"
#include "Interface/VNMiniGameControlInterface.h"
#include "VNMiniGameMode.generated.h"



/**
 * 
 */
UCLASS()
class VN_API AVNMiniGameMode : public AVNGameModeBase, public IVNMiniGameControlInterface
{
	GENERATED_BODY()

public:
	virtual void PlayerLose() override;

	virtual EMiniGameClearType GetMiniGameType() const override;
	
protected:
	virtual void BeginPlay() override;
	
	/* 미니 게임 호감도 보상 */
	UPROPERTY(EditDefaultsOnly, Category = "Clear Info")
	TMap<FName /* 관련된 Character ID */, int32 /* 호감도 보상 */> AffectionRewardMap;

	void PlayerWin() const;
	

	
};
