// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Actor/SpawnSystem/VNEnemySpawnVolume.h"

#include "DebugHelper.h"
#include "NavigationSystem.h"
#include "VNMiniGameFunctionLibrary.h"
#include "Actor/SpawnSystem/VNEnemySpawnPoint.h"
#include "Character/VNEnemyCharacter.h"
#include "Components/BoxComponent.h"
#include "Interface/VNEnemyDefeatGameModeInterface.h"
#include "Interface/VNPlayerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/Pooling/VNObjectPoolManager.h"
#include "GameFramework/GameModeBase.h"


AVNEnemySpawnVolume::AVNEnemySpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	SetRootComponent(BoxCollision);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
}


void AVNEnemySpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
	BoxCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnSphereBeginOverlap);

	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
	if (IVNEnemyDefeatGameModeInterface* DefeatGameMode = Cast<IVNEnemyDefeatGameModeInterface>(GameMode))
	{
		DefeatGameMode->RegisterSpawnVolume(this);
	}
	
	
}

void AVNEnemySpawnVolume::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->Implements<UVNPlayerInterface>()) return;

	BoxCollision->OnComponentBeginOverlap.RemoveAll(this);

	if (SpawnPoints.IsEmpty()) return;

	UVNObjectPoolManager* PoolManager = UVNMiniGameFunctionLibrary::GetPoolManger(this);
	if (!PoolManager)
	{
		DebugHelper::Print(TEXT("Pool Manager is NOT valid"), FColor::Yellow);
		return;
	}

	FTransform SpawnTransform = FTransform::Identity;
	FVector RandomLocation = FVector::ZeroVector;

	for (const AVNEnemySpawnPoint* SpawnPoint : SpawnPoints)
	{
		if (SpawnPoint->EnemyParams.EnemyClass == nullptr) continue;

		SpawnTransform = SpawnPoint->GetActorTransform();
		
		RandomLocation = FVector::ZeroVector;
		if (UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(this, SpawnTransform.GetLocation(), RandomLocation, SpawnRadius))
		{
			SpawnTransform.SetLocation(RandomLocation);
		}

		AActor* PooledObject = PoolManager->GetPooledObject(SpawnPoint->EnemyParams.EnemyClass);
		if (!PooledObject)
		{
			DebugHelper::Print(TEXT("Object is NOT valid"), FColor::Yellow);
			continue;
		}

		AVNEnemyCharacter* Enemy = CastChecked<AVNEnemyCharacter>(PooledObject);
		if (!Enemy->GetCharacterDiedDelegate().IsBoundToObject(this))
		{
			Enemy->GetCharacterDiedDelegate().AddUObject(this, &ThisClass::OnEnemyDied);
		}
		
		Enemy->SpawnEnemy(SpawnTransform);
		NumSpawnedEnemy++;
		
	}
	
	
	
}

void AVNEnemySpawnVolume::OnEnemyDied()
{
	if (NumSpawnedEnemy == 0)
	{
		DebugHelper::Print(TEXT("The number of spawned enemies is already 0. maybe something is wrong"), FColor::Yellow);
		return;
	}

	NumSpawnedEnemy--;
	if (0 < NumSpawnedEnemy) return;

	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
	if (IVNEnemyDefeatGameModeInterface* DefeatGameMode = Cast<IVNEnemyDefeatGameModeInterface>(GameMode))
	{
		DefeatGameMode->SpawnVolumeCleared(this);
	}
	
	
}

