// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VNEnemySpawnVolume.generated.h"

class AVNEnemySpawnPoint;
class ATargetPoint;
class AVNEnemyCharacter;
class UBoxComponent;



UCLASS()
class VNMINIGAME_API AVNEnemySpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	AVNEnemySpawnVolume();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> BoxCollision;
	
	/* 스폰 지점 */
	UPROPERTY(EditAnywhere, Category = "Spawn Properties")
	TArray<AVNEnemySpawnPoint*> SpawnPoints;

	/* SpawnPoint로 부터소환 가능한 최대 거리 */
	float SpawnRadius = 50.f;

private:
	void GetValidSpawnLocation(FVector& OutLocation) const;
	void OnEnemyDied();
	int32 NumSpawnedEnemy = 0;

};
