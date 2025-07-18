// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/PoolableActorInterface.h"
#include "VNAutoPickupItem.generated.h"

class UBoxComponent;

DECLARE_MULTICAST_DELEGATE(FOnBoxBeginOverlap);

UCLASS()
class VNMINIGAME_API AVNAutoPickupItem : public AActor, public IPoolableActorInterface
{
	GENERATED_BODY()
	
public:	
	AVNAutoPickupItem();
	void SpawnItem(const FVector& SpawnLocation);

	/* begin IPoolableActorInterface */
	virtual void OnActivateFromPool_Implementation() override;
	virtual void OnDeactivateToPool_Implementation() override;
	/* end IPoolableActorInterface */

	FOnBoxBeginOverlap OnBoxBeginOverlapDelegate;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> BoxCollision;
	
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
