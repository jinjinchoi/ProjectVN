// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Manager/Pooling/VNObjectPoolManager.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/PoolableActorInterface.h"


void UVNObjectPoolManager::InitEnemyDefeatObject()
{
	CreateObjectPool(EnemyDefeatObjectClasses);
}

void UVNObjectPoolManager::InitCrossingObject()
{
	CreateObjectPool(CrossingObjectClasses);
}

void UVNObjectPoolManager::CreateObjectPool(const TArray<TSubclassOf<AActor>>& InObjectClasses)
{
	for (const TSubclassOf<AActor>& ObjClass : InObjectClasses)
	{
		TArray<AActor*>& Pool = ObjectPools.FindOrAdd(ObjClass);
		for (int32 i = 0; i < PoolSizePerType; ++i)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			
			if (AActor* PoolActor = GetWorld()->SpawnActor<AActor>(ObjClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams))
			{
				PoolActor->SetActorHiddenInGame(true);
				PoolActor->SetActorEnableCollision(false);
				PoolActor->SetActorTickEnabled(false);

				if (const ACharacter* Character = Cast<ACharacter>(PoolActor))
				{
					Character->GetCharacterMovement()->StopMovementImmediately();
					Character->GetCharacterMovement()->GravityScale = 0.f;
				}
				
				Pool.Add(PoolActor);
			}
		}
	}
}

AActor* UVNObjectPoolManager::GetPooledObject(const TSubclassOf<AActor>& ActorClass)
{
	if (ActorClass == nullptr) return nullptr;

	TArray<AActor*>* Pool = ObjectPools.Find(ActorClass);
	if (Pool == nullptr) return nullptr;

	for (AActor* PoolActor : *Pool)
	{
		if (IsValid(PoolActor) && PoolActor->IsHidden())
		{
			if (const ACharacter* Character = Cast<ACharacter>(PoolActor))
			{
				Character->GetCharacterMovement()->GravityScale = 1.f;
			}
			
			if (PoolActor->GetClass()->ImplementsInterface(UPoolableActorInterface::StaticClass()))
			{
				IPoolableActorInterface::Execute_OnActivateFromPool(PoolActor);
			}
			
			return PoolActor;
		}
	}

	if (AActor* NewActor = GetWorld()->SpawnActor<AActor>(ActorClass, FVector::ZeroVector, FRotator::ZeroRotator))
	{
		ObjectPools[ActorClass].Add(NewActor);
		if (NewActor->GetClass()->ImplementsInterface(UPoolableActorInterface::StaticClass()))
		{
			IPoolableActorInterface::Execute_OnActivateFromPool(NewActor);
		}
		return NewActor;
	}

	return nullptr;
	
}

void UVNObjectPoolManager::ReturnObject(AActor* ActorToReturn)
{
	if (!IsValid(ActorToReturn)) return;
	
	ActorToReturn->SetActorHiddenInGame(true);
	ActorToReturn->SetActorEnableCollision(false);
	ActorToReturn->SetActorTickEnabled(false);

	if (const ACharacter* Character = Cast<ACharacter>(ActorToReturn))
	{
		Character->GetCharacterMovement()->GravityScale = 0.f;
		UAnimInstance* OwningAnimInstance = Character->GetMesh()->GetAnimInstance();

		if (OwningAnimInstance && OwningAnimInstance->IsAnyMontagePlaying())
		{
			OwningAnimInstance->StopAllMontages(0.1f);
		}
	}
	
	
	if (ActorToReturn->GetClass()->ImplementsInterface(UPoolableActorInterface::StaticClass()))
	{
		IPoolableActorInterface::Execute_OnDeactivateToPool(ActorToReturn);
	}
	
}

void UVNObjectPoolManager::ClearPool()
{
	for (TPair<UClass*, TArray<AActor*>>& PoolMap : ObjectPools)
	{
		for (AActor* PoolActor : PoolMap.Value)
		{
			if (IsValid(PoolActor))
			{
				PoolActor->Destroy();
			}
		}
	}

	ObjectPools.Empty();
	
}
