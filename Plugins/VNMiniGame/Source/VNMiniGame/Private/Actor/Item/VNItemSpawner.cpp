// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Actor/Item/VNItemSpawner.h"

#include "DebugHelper.h"
#include "VNMiniGameFunctionLibrary.h"
#include "Actor/Item/VNAutoPickupItem.h"
#include "Algo/RandomShuffle.h"
#include "Interface/VNRoadCrossingGameModeInterface.h"
#include "Manager/Pooling/VNObjectPoolManager.h"
#include "GameFramework/GameModeBase.h"

void AVNItemSpawner::BeginPlay()
{
	Super::BeginPlay();

	IVNRoadCrossingGameModeInterface* RoadCrossingGame = Cast<IVNRoadCrossingGameModeInterface>(GetWorld()->GetAuthGameMode());
	if (RoadCrossingGame == nullptr) return;
	RoadCrossingGame->RegisterItemSpawner(this);

	if (ItemSpawnPoints.Num() < TargetSpawnCount)
	{
		TargetSpawnCount = ItemSpawnPoints.Num();
	}
	
}


void AVNItemSpawner::SpawnItem()
{
	if (ItemSpawnPoints.IsEmpty()) return;
	
	UVNObjectPoolManager* PoolManager = UVNMiniGameFunctionLibrary::GetPoolManger(this);
	IVNRoadCrossingGameModeInterface* RoadCrossingGame = Cast<IVNRoadCrossingGameModeInterface>(GetWorld()->GetAuthGameMode());
	
	if (PoolManager == nullptr)
	{
		RoadCrossingGame->GetPoolManagerSetDelegate().AddLambda([this, RoadCrossingGame]()
		{
			SpawnItem();
		});
		return;
	}

	if (CurrentSpawnCount != PickedUpItemCount)
	{
		FString DebugMsg = FString::Printf(TEXT("Not all items have been collected yet:  %hs"), __FUNCTION__);
		DebugHelper::Print(TEXT("모든 아이템을 획득한 뒤 아이템을 다시 스폰해야합니다."), FColor::Red);
		return;
	}

	// 소환된 아이템의 숫자 초기화. 아이템은 제일 처음 레벨이 시작될때와 단계 클리어할때만 스폰됨.
	CurrentSpawnCount = 0;
	PickedUpItemCount = 0;
	
	Algo::RandomShuffle(ItemSpawnPoints);

	DebugHelper::Print(TEXT("아이템 스폰!"), FColor::Green);
	
	for (int32 i = 0; i < TargetSpawnCount; i++)
	{
		AActor* PooledActor = PoolManager->GetPooledObject(ItemClass);
		if (!IsValid(PooledActor))
		{
			FString DebugMsg = FString::Printf(TEXT("Pickup Item Not Valid this func: %hs"), __FUNCTION__);
			DebugHelper::Print(DebugMsg, FColor::Yellow);
			return;
		}
		
		AVNAutoPickupItem* AutoPickupItem = Cast<AVNAutoPickupItem>(PooledActor);
		if (!IsValid(AutoPickupItem)) return;

		// 아이템 오버랩 바인딩
		AutoPickupItem->OnBoxBeginOverlapDelegate.AddLambda([this, AutoPickupItem, PoolManager]()
		{
			PoolManager->ReturnObject(AutoPickupItem);
			OnPickedUpItem();
		});
		
		AutoPickupItem->SpawnItem(ItemSpawnPoints[i]->GetActorLocation());
		CurrentSpawnCount++;
	}
	
}

void AVNItemSpawner::OnPickedUpItem()
{
	PickedUpItemCount++;
	if (PickedUpItemCount >= CurrentSpawnCount)
	{
		IVNRoadCrossingGameModeInterface* RoadCrossingGame = Cast<IVNRoadCrossingGameModeInterface>(GetWorld()->GetAuthGameMode());
		RoadCrossingGame->AllItemPickedUp();
	}
}