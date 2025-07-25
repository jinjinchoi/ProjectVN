// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "VNItemSpawner.generated.h"

class AVNAutoPickupItem;
/**
 * 
 */
UCLASS()
class VNMINIGAME_API AVNItemSpawner : public AActor
{
	GENERATED_BODY()

public:
	void SpawnItem();
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "User Properties")
	TArray<ATargetPoint*> ItemSpawnPoints;
	
	UPROPERTY(EditAnywhere, Category = "User Properties")
	TSubclassOf<AActor> ItemClass;

	UPROPERTY(EditAnywhere, Category = "User Properties")
	int32 TargetSpawnCount = 5;

private:
	void OnPickedUpItem();
	
	// 월드에 스폰된 아이템의 수.
	int32 CurrentSpawnCount = 0;
	// 플레이어가 획득한 아이템의 수
	int32 PickedUpItemCount = 0;
	
};
