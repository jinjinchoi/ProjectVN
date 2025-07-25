// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Actor/VehicleVolume/VNVehicleSpawnVolume.h"

#include "DebugHelper.h"
#include "VNMiniGameFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/VNRoadCrossingGameModeInterface.h"
#include "Interface/VNVehicleInterface.h"
#include "Manager/Pooling/VNObjectPoolManager.h"

AVNVehicleSpawnVolume::AVNVehicleSpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	BoxCollision->SetupAttachment(GetRootComponent());
	BoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Overlap);
	
}


void AVNVehicleSpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
	IVNRoadCrossingGameModeInterface* RoadCrossingGame = Cast<IVNRoadCrossingGameModeInterface>(GetWorld()->GetAuthGameMode());

	// 디버그 메세지
	if (!RoadCrossingGame)
	{
		const FString ClassName = GetWorld()->GetAuthGameMode() ? GetWorld()->GetAuthGameMode()->GetClass()->GetName() : TEXT("None");
		const FString FunctionName = FString(__FUNCTION__);
		const FString Msg = TEXT("GameMode 인터페이스 캐스팅 실패하였습니다. 클래스: ") + ClassName + TEXT(", 호출 위치: ") + FunctionName;
		return;
	}
	
	RoadCrossingGame->GetOnLevelStartedDelegate().AddLambda([this, RoadCrossingGame]()
	{
		SpawnLevel = RoadCrossingGame->GetCurrentMapLevel();
	});

	ActivateSpawnVolume();
	
	
}

void AVNVehicleSpawnVolume::ActivateSpawnVolume()
{
	if (VehicleClasses.IsEmpty() || Road.IsEmpty()) return;
	
	PoolManager = UVNMiniGameFunctionLibrary::GetPoolManger(this);
	if (PoolManager)
	{
		StartSpawnTimer();
		return;
	}
	
	IVNRoadCrossingGameModeInterface* RoadCrossingGame = Cast<IVNRoadCrossingGameModeInterface>(GetWorld()->GetAuthGameMode());
	if (!RoadCrossingGame) return;
	
	RoadCrossingGame->GetPoolManagerSetDelegate().AddLambda([this]()
	{
		PoolManager = UVNMiniGameFunctionLibrary::GetPoolManger(this);
		if (!PoolManager || VehicleClasses.IsEmpty() || Road.IsEmpty()) return;
		StartSpawnTimer();
	});
	
}


void AVNVehicleSpawnVolume::StartSpawnTimer()
{
	const float Interval = GetSpawnTime();

	GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &ThisClass::SpawnVehicle, Interval, false);
	
}

void AVNVehicleSpawnVolume::SpawnVehicle()
{
	if (IsSpawnLocationOccupied())
	{
		StartSpawnTimer();
		return;
	}
	
	const TSubclassOf<AActor> SelectedClass = VehicleClasses[FMath::RandRange(0, VehicleClasses.Num() - 1)];

	if (AActor* PooledObject = PoolManager->GetPooledObject(SelectedClass))
	{
		if (!PooledObject)
		{
			DebugHelper::Print(TEXT("Object is NOT valid"), FColor::Yellow);
			StartSpawnTimer();
			return;
		}

		if (IVNVehicleInterface* VehicleInterface = Cast<IVNVehicleInterface>(PooledObject))
		{
			FTransform ActorTransform = GetActorTransform();
			ActorTransform.SetScale3D(FVector(1.f, 1.f, 1.f));

			VehicleInterface->SetRoad(Road);
			VehicleInterface->VehicleSpawn(ActorTransform);
		}
	}

	StartSpawnTimer();

}

float AVNVehicleSpawnVolume::GetSpawnTime() const
{
	for (const FSpawnTimeParams& Params : SpawnTimeParams)
	{
		if (Params.SpawnLevel == SpawnLevel)
		{
			return Params.SpawnTime;
		}
	}

	return 3.f;
}

bool AVNVehicleSpawnVolume::IsSpawnLocationOccupied() const
{
	TArray<AActor*> OverlappingActors;
	BoxCollision->GetOverlappingActors(OverlappingActors, AActor::StaticClass());

	for (const AActor* OverlappingActor : OverlappingActors)
	{
		if (OverlappingActor->ActorHasTag("Vehicle"))
		{
			return true;
		}
	}

	return false;
}


