// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Actor/Projectile/MultiHitProjectile.h"

#include "DebugHelper.h"
#include "Interface/VNCombatInterface.h"

void AMultiHitProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMultiHitProjectile::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UVNCombatInterface>())
	{
		OverlappedActors.Add(OtherActor); 
	}
}

void AMultiHitProjectile::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OverlappedActors.Remove(OtherActor);
}

void AMultiHitProjectile::OnActivateFromPool_Implementation()
{
	Super::OnActivateFromPool_Implementation();
	
	GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &ThisClass::ApplyPeriodicDamage, DamageInterval, true);
	
}

void AMultiHitProjectile::OnDeactivateToPool_Implementation()
{
	Super::OnDeactivateToPool_Implementation();

	GetWorldTimerManager().ClearTimer(DamageTimerHandle);
	
}

void AMultiHitProjectile::ApplyPeriodicDamage()
{
	if (!GetOwner())
	{
		DebugHelper::Print(TEXT("Owner Not Valid"), FColor::Yellow);
		return;
	}
	
	for (TSet<TWeakObjectPtr<AActor>>::TIterator It = OverlappedActors.CreateIterator(); It; ++It)
	{
		if (!It->IsValid())
		{
			It.RemoveCurrent();
			continue;
		}
		
		AActor* Target = It->Get();
		if (IsTargetHostile(Target))
		{
			OnHit(Target->GetActorLocation());
			ApplyDamageToTarget(Target);
		}
		
	}
	
}
