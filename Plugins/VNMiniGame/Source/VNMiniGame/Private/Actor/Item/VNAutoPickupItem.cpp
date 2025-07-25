// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Actor/Item/VNAutoPickupItem.h"

#include "Components/BoxComponent.h"

AVNAutoPickupItem::AVNAutoPickupItem()
{

	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh Component");
	SetRootComponent(StaticMeshComponent);
	StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	BoxCollision->SetupAttachment(GetRootComponent());
	BoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

}

void AVNAutoPickupItem::SpawnItem(const FVector& SpawnLocation)
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorLocation(SpawnLocation);
}

void AVNAutoPickupItem::OnActivateFromPool_Implementation()
{
	
}

void AVNAutoPickupItem::OnDeactivateToPool_Implementation()
{
	
}


void AVNAutoPickupItem::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnBoxBeginOverlap);
	
}

void AVNAutoPickupItem::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->ActorHasTag("Player")) return;

	OnBoxBeginOverlapDelegate.Broadcast();
	
	
}

