// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Actor/VehicleVolume/VNVehicleDestinationVolume.h"

#include "DebugHelper.h"
#include "VNMiniGameFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/VNRoadCrossingGameModeInterface.h"
#include "Interface/VNVehicleInterface.h"
#include "Manager/Pooling/VNObjectPoolManager.h"

AVNVehicleDestinationVolume::AVNVehicleDestinationVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(BoxCollision);

}

void AVNVehicleDestinationVolume::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnSphereBeginOverlap);

	PoolManager = UVNMiniGameFunctionLibrary::GetPoolManger(this);
	if (!PoolManager)
	{
		IVNRoadCrossingGameModeInterface* RoadCrossingGame = Cast<IVNRoadCrossingGameModeInterface>(GetWorld()->GetAuthGameMode());

		// 디버그 메세지
		if (!RoadCrossingGame)
		{
			const FString ClassName = GetWorld()->GetAuthGameMode() ? GetWorld()->GetAuthGameMode()->GetClass()->GetName() : TEXT("None");
			const FString FunctionName = FString(__FUNCTION__);
			const FString Msg = TEXT("GameMode 인터페이스 캐스팅 실패하였습니다. 클래스: ") + ClassName + TEXT(", 호출 위치: ") + FunctionName;
			return;
		}

		RoadCrossingGame->GetPoolManagerSetDelegate().AddLambda([this]()
		{
			PoolManager = UVNMiniGameFunctionLibrary::GetPoolManger(this);
		});
	}
	
}

void AVNVehicleDestinationVolume::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->Implements<UVNVehicleInterface>()) return;

	if (!PoolManager)
	{
		OtherActor->Destroy();
	}

	PoolManager->ReturnObject(OtherActor);
	
}

