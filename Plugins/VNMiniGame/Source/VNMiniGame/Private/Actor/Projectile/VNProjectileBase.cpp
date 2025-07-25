// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Actor/Projectile/VNProjectileBase.h"

#include "DebugHelper.h"
#include "GenericTeamAgentInterface.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "VNMiniGameFunctionLibrary.h"
#include "Component/Combat/VNStatComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interface/VNCombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/Pooling/VNObjectPoolManager.h"

AVNProjectileBase::AVNProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SphereCollision = CreateDefaultSubobject<USphereComponent>("Sphere Collision");
	SetRootComponent(SphereCollision);

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Projectile Effect Component"));
	NiagaraComponent->SetupAttachment(GetRootComponent());
	NiagaraComponent->bAutoActivate = false;
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement Component"));
	ProjectileMovementComponent->StopMovementImmediately();
	ProjectileMovementComponent->SetComponentTickEnabled(false);
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;

}

void AVNProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
	SphereCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnSphereBeginOverlap);
	SphereCollision->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnSphereEndOverlap);
	
}


void AVNProjectileBase::OnActivateFromPool_Implementation()
{
	GetWorldTimerManager().SetTimer(ProjectileLifeSpanTimer, this, &ThisClass::ProjectileRemove, LifeSpan, false);
	
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	SphereCollision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	SphereCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	if (!LoopingSoundComponent && LoopingSound)
	{
		LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
		LoopingSoundComponent->bStopWhenOwnerDestroyed = true;
	}
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Play();
	}

	ProjectileMovementComponent->SetComponentTickEnabled(true);
	ProjectileMovementComponent->InitialSpeed =InitSpeed;
	ProjectileMovementComponent->MaxSpeed = InitSpeed;

	if (NiagaraComponent)
	{
		NiagaraComponent->Activate(true);
	}
}

void AVNProjectileBase::OnDeactivateToPool_Implementation()
{
	GetWorldTimerManager().ClearTimer(ProjectileLifeSpanTimer);
	
	ProjectileMovementComponent->StopMovementImmediately();
	ProjectileMovementComponent->SetComponentTickEnabled(false);
	ProjectileMovementComponent->bRotationFollowsVelocity = false;
	
	if (ProjectileMovementComponent->bIsHomingProjectile)
	{
		ProjectileMovementComponent->bIsHomingProjectile = false;
		ProjectileMovementComponent->HomingTargetComponent = nullptr;
	}
	
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (NiagaraComponent)
	{
		NiagaraComponent->Deactivate();
	}

	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
	}
	
	if (IVNCombatInterface* CombatInterface = Cast<IVNCombatInterface>(HomingTargetActor))
	{
		CombatInterface->GetCharacterDiedDelegate().RemoveAll(this);
		HomingTargetActor = nullptr;
	}

	CachedInstigatorController.Reset();
	CachedDamageCauser.Reset();
	InstigatorTeamID = 255;
	ProjectileDamage = 0.0f;
	
	SetOwner(nullptr);
}

bool AVNProjectileBase::InitProjectile(AActor* InOwner,  const float DamageCoefficient)
{
	if (!InOwner)
	{
		DebugHelper::Print(TEXT("Owner not valid"), FColor::Yellow);
		return false;
	}

	SetOwner(InOwner);
	CachedDamageCauser = InOwner;
	CachedInstigatorController = InOwner->GetInstigatorController();

	if (const UVNStatComponent* OwnerStatComponent = IVNCombatInterface::Execute_GetStatComponent(InOwner))
	{
		ProjectileDamage = OwnerStatComponent->GetAttackPower() * DamageCoefficient;
	}
	
	if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(InOwner->GetInstigatorController()))
	{
		InstigatorTeamID = TeamAgent->GetGenericTeamId().GetId();
	}

	return true;
	
}

void AVNProjectileBase::SetHomingTarget(AActor* Target, const bool bRotationFollowsVelocity)
{
	if (Target == nullptr) return;

	HomingTargetActor = Target;
	ProjectileMovementComponent->bIsHomingProjectile = true;
	ProjectileMovementComponent->HomingTargetComponent = HomingTargetActor->GetRootComponent();
	ProjectileMovementComponent->bRotationFollowsVelocity = bRotationFollowsVelocity;
	ProjectileMovementComponent->HomingAccelerationMagnitude = HomingAccelerationMagnitude;
	if (IVNCombatInterface* CombatInterface = Cast<IVNCombatInterface>(HomingTargetActor))
	{
		CombatInterface->GetCharacterDiedDelegate().AddUObject(this, &ThisClass::OnTargetDestroyed);
	}
}


void AVNProjectileBase::LaunchProjectile(const FVector& SpawnLocation, const FVector& TargetLocation)
{
	
	FVector FlatTargetLocation = TargetLocation;
	FlatTargetLocation.Z = SpawnLocation.Z;
	const FVector Direction = (FlatTargetLocation - SpawnLocation).GetSafeNormal();
	
	SetActorLocation(SpawnLocation);
	SetActorRotation(Direction.Rotation());
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	
	ProjectileMovementComponent->Velocity = InitSpeed * Direction;
	ProjectileMovementComponent->UpdateComponentVelocity();
}


void AVNProjectileBase::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void AVNProjectileBase::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

bool AVNProjectileBase::IsTargetHostile(const AActor* OtherTarget) const
{
	if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherTarget->GetInstigatorController()))
	{
		const uint8 TargetTeamID = TeamAgent->GetGenericTeamId().GetId();
		return InstigatorTeamID != TargetTeamID;
	}

	return false;
}


void AVNProjectileBase::ApplyDamageToTarget(AActor* DamagedActor) const
{
	if (!DamagedActor) return;
	if (!DamagedActor->Implements<UVNCombatInterface>() || IVNCombatInterface::Execute_IsDead(DamagedActor)) return;

	if (IsTargetHostile(DamagedActor))
	{
		UGameplayStatics::ApplyDamage(
			DamagedActor,
			ProjectileDamage,
			CachedInstigatorController.Get(),
			CachedDamageCauser.Get(),
			UDamageType::StaticClass()
		);
		
	}

}

void AVNProjectileBase::OnHit(const FVector& LocationOverride) const
{
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), GetActorRotation());
	}

	if (ImpactEffect)
	{
		if (LocationOverride != FVector::ZeroVector)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, LocationOverride, GetActorRotation());
		}
		else
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation(), GetActorRotation());
		}
	}
	
}

void AVNProjectileBase::ProjectileRemove()
{
	if (DestructionEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, DestructionEffect, GetActorLocation(), GetActorRotation());
	}

	if (DestructionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DestructionSound, GetActorLocation(), GetActorRotation());
	}
	
	if (UVNObjectPoolManager* PoolManager = UVNMiniGameFunctionLibrary::GetPoolManger(this))
	{
		PoolManager->ReturnObject(this);
	}
}

void AVNProjectileBase::OnTargetDestroyed()
{
	if (IVNCombatInterface* CombatInterface = Cast<IVNCombatInterface>(HomingTargetActor))
	{
		CombatInterface->GetCharacterDiedDelegate().RemoveAll(this);
		HomingTargetActor = nullptr;
	}
	
	// bAutoRetarget이 설정되어 있으면 오너로부터 가장 가까운 적을 찾아 호밍 타겟을 재설정.
	if (bAutoRetarget && ProjectileMovementComponent->bIsHomingProjectile && GetOwner())
	{
		TArray<AActor*> OverlappedActors;
		TArray<AActor*> IgnoreActors;
		IgnoreActors.Add(GetOwner());
		IgnoreActors.Add(this);
		
		UVNMiniGameFunctionLibrary::GetHostileActorsWithinRadius(
			this, GetOwner(), OverlappedActors, IgnoreActors, RetargetRadius, GetActorLocation()
		);

		if (OverlappedActors.Num() > 0)
		{
			HomingTargetActor = UVNMiniGameFunctionLibrary::GetClosestActor(OverlappedActors, GetActorLocation());
			
			ProjectileMovementComponent->HomingTargetComponent = HomingTargetActor->GetRootComponent();

			return;
		}
	}
	
	// 오토 타겟이 아니거나 그 외 유효하지 않는 상황에서는 발사체 제거
	ProjectileRemove();
	
}


