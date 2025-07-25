// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Pawn/Vehicle/VNVehiclePawnAI.h"
#include "VNMiniGameFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Interface/VNCombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/Pooling/VNObjectPoolManager.h"
#include "ChaosWheeledVehicleMovementComponent.h"

AVNVehiclePawnAI::AVNVehiclePawnAI()
{
	AutoPossessAI = EAutoPossessAI::Disabled;

	GetMesh()->bReceivesDecals = false;
	GetMesh()->SetSimulatePhysics(false);

	FrontSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Front Scene"));
	FrontSceneComponent->SetupAttachment(GetMesh());

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	BoxCollision->SetupAttachment(GetMesh());
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	BoxCollision->SetGenerateOverlapEvents(true);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	AudioComponent->SetupAttachment(GetRootComponent());
	AudioComponent->bStopWhenOwnerDestroyed = true;
	AudioComponent->bAutoActivate = false;
	
	
}

void AVNVehiclePawnAI::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(TEXT("Vehicle"));

	BoxCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnBoxBeginOverlap);
	
}


void AVNVehiclePawnAI::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->ActorHasTag("Player")) return;
	if (RecentlyHitActors.Contains(OtherActor)) return;
	
	
	const float ForwardSpeed =  GetCurrentSpeed();
	const FVector HitDirection = (OtherActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();

	// Dot > 0.7  : 정면
	// 0.7 >= Dot >= 0.7  :측면
	// Dot < 0.7 : 후면
	const float Dot = FVector::DotProduct(GetActorForwardVector(), HitDirection);

	// 속도(K/h) 와 충돌 방향을 기준으로 대미지 설정.
	const float Damage = FMath::Clamp(ForwardSpeed * Dot, 30, 100); 

	UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());

	const FVector KnockbackVelocity = HitDirection * (ForwardSpeed * 20) + FVector(0, 0, 350 * Dot);
	
	IVNCombatInterface::Execute_LaunchCombatCharacter(OtherActor, KnockbackVelocity, true, true, GetActorLocation(), false);

	if (CrashSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(this, CrashSound, GetFrontLocationOfCar());
	}
	
	RecentlyHitActors.Add(OtherActor);

	FTimerHandle TimerHandle;
	FTimerDelegate TimerDel;

	TWeakObjectPtr<AVNVehiclePawnAI> WeakThis(this);
	TWeakObjectPtr<AActor> WeakOtherActor(OtherActor);
	
	TimerDel.BindLambda([WeakThis, WeakOtherActor]()
	{
		if (!WeakThis.IsValid() || !WeakOtherActor.IsValid()) return;
		
		WeakThis.Get()->RecentlyHitActors.Remove(WeakOtherActor.Get());
	});

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 1.5f, false);
	
	
}

#pragma region IVNVehicleInterface
void AVNVehiclePawnAI::SetThrottle(const float InThrottle)
{
	GetVehicleMovementComponent()->SetThrottleInput(InThrottle);
}

void AVNVehiclePawnAI::SetBrake(const float InBrake)
{
	GetVehicleMovementComponent()->SetBrakeInput(InBrake);
}

void AVNVehiclePawnAI::SetSteering(const float InSteering)
{
	GetVehicleMovementComponent()->SetSteeringInput(InSteering);
}

void AVNVehiclePawnAI::SetRoad(const TArray<AActor*>& InRoad)
{
	Road = InRoad;
	if (Road.Num() > 1)
	{
		RoadIndex = FMath::RandRange(0, Road.Num() - 1);
	}
}

void AVNVehiclePawnAI::SetRoadIndex(const int32 InRoadIndex)
{
	RoadIndex = InRoadIndex;
}

AActor* AVNVehiclePawnAI::GetCurrentRoad() const
{
	if (Road.IsEmpty()) return nullptr;

	return Road[RoadIndex];
}

TArray<AActor*> AVNVehiclePawnAI::GetAllRoad() const
{
	return Road;
}

int32 AVNVehiclePawnAI::GetCurrentLane() const
{
	return RoadIndex;
}

FVector AVNVehiclePawnAI::GetFrontLocationOfCar() const
{
	return FrontSceneComponent->GetComponentLocation();
}

float AVNVehiclePawnAI::GetCurrentSpeed() const
{
	return GetVehicleMovementComponent()->GetForwardSpeed() * 0.036f;
}

FVector AVNVehiclePawnAI::GetForwardVector() const
{
	return GetActorForwardVector();
}

void AVNVehiclePawnAI::VehicleSpawn(const FTransform& TransformToSpawn)
{
	SetActorHiddenInGame(false);
	SetActorTransform(TransformToSpawn);
	SetActorEnableCollision(true);
	
	GetMesh()->SetSimulatePhysics(true);
	
	if (CachedController)
	{
		CachedController->Possess(this);
	}
	else
	{
		SpawnDefaultController();
		CachedController = GetController();
	}

	if (AudioComponent)
	{
		AudioComponent->Play();
	}
}

#pragma endregion

#pragma region IPoolableActorInterface

void AVNVehiclePawnAI::OnActivateFromPool_Implementation()
{
	TWeakObjectPtr<AActor> WeakThis(this);
	GetWorldTimerManager().SetTimer(LifeTimeHandle, FTimerDelegate::CreateLambda([WeakThis]()
	{
		if (!WeakThis.IsValid()) return;
		
		 if (UVNObjectPoolManager* PoolManager = UVNMiniGameFunctionLibrary::GetPoolManger(WeakThis.Get()))
		 {
			 PoolManager->ReturnObject(WeakThis.Get());
		 }
	}), LifeTime, false);
	
}

void AVNVehiclePawnAI::OnDeactivateToPool_Implementation()
{
	GetMesh()->SetSimulatePhysics(false);
	
	if (AController* AIController = GetController())
	{
		AIController->UnPossess();
	}

	GetWorldTimerManager().ClearTimer(LifeTimeHandle);

	if (AudioComponent)
	{
		AudioComponent->Stop();
	}

	RoadIndex = 0;
	Road.Empty();
	RecentlyHitActors.Empty();
	
}

#pragma endregion 