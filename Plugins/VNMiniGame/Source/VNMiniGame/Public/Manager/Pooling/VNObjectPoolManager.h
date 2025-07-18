// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VNObjectPoolManager.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class VNMINIGAME_API UVNObjectPoolManager : public UObject
{
	GENERATED_BODY()

public:
	/* ObjectClasses의 정보를 바탕으로 ObjectPools 맵에 실제 오브젝트를 생성하여 저장 */
	void InitEnemyDefeatObject();
	void InitCrossingObject();

	/* 클래스를 기반으로 오브젝트를 찾아오는 함수. 풀에 오브젝트가 존재하지 않으면 새로 만들어서 반환 */
	AActor* GetPooledObject(const TSubclassOf<AActor>& ActorClass);
	/* 오브젝트 풀에 액터를 돌려 넣는 함수 */
	void ReturnObject(AActor* ActorToReturn);

	void ClearPool();
	
protected:
	/* 오브젝트 풀에 미리 생성해 놓을 클래스 타입들 */
	UPROPERTY(EditDefaultsOnly, Category = "Pooling", meta = (MustImplement = "PoolableActorInterface"))
	TArray<TSubclassOf<AActor>> EnemyDefeatObjectClasses;

	UPROPERTY(EditDefaultsOnly, Category = "Pooling", meta = (MustImplement = "PoolableActorInterface"))
	TArray<TSubclassOf<AActor>> CrossingObjectClasses;

	UPROPERTY(EditDefaultsOnly, Category = "Pooling")
	int32 PoolSizePerType = 5;

private:
	void CreateObjectPool(const TArray<TSubclassOf<AActor>>& InObjectClasses);
	
	/* 오브젝트를 저장하는 Tap으로 클래스와 액터를 매칭하여 저장. */
	TMap<UClass*, TArray<AActor*>> ObjectPools;
	
};
