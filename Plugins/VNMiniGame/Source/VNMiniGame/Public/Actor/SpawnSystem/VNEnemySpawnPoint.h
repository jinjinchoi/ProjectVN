// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "VNEnemySpawnPoint.generated.h"

class AVNEnemyCharacter;

USTRUCT(BlueprintType)
struct FEnemyInfoParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<AVNEnemyCharacter> EnemyClass = nullptr;

	UPROPERTY(EditAnywhere)
	int32 EnemyLevel = 1;
	
};

/**
 * 
 */
UCLASS()
class VNMINIGAME_API AVNEnemySpawnPoint : public ATargetPoint
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Spawn Properties")
	FEnemyInfoParams EnemyParams;

	
};
