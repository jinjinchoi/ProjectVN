// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Actor/Projectile/SingleHitProjectile.h"

#include "VNMiniGameFunctionLibrary.h"
#include "Interface/VNCombatInterface.h"

void ASingleHitProjectile::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor)) return;
	
	if (OtherActor->Implements<UVNCombatInterface>())
	{
		if (IsTargetHostile(OtherActor))
		{
			OnHit();
			ApplyDamageToTarget(OtherActor);
			ProjectileRemove();
		}
	}
	else
	{
		OnHit();
		ProjectileRemove();
	}
	
}

void ASingleHitProjectile::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}
