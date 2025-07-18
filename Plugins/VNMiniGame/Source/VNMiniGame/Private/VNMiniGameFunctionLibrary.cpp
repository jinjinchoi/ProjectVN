// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "VNMiniGameFunctionLibrary.h"

#include "GenericTeamAgentInterface.h"
#include "Engine/OverlapResult.h"
#include "Interface/VNCombatInterface.h"
#include "Interface/VNPoolProviderInterface.h"

void UVNMiniGameFunctionLibrary::GetLiveActorWithinRadius(const UObject* WorldContextObject,
                                                          TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
                                                          const FVector& SphereOrigin, const bool bDrawDebugSphere)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);
	
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		
		if (bDrawDebugSphere)
		{
			// Origin 위치 메세지로 표시
			GEngine->AddOnScreenDebugMessage(
				-1,
				2.f,
				FColor::Cyan,
				FString::Printf(TEXT("SphereOrigin: %s"), *SphereOrigin.ToString())
			);

			// 범위 구형으로 표시
			DrawDebugSphere(
				World,
				SphereOrigin,
				Radius,
				16, // 세그먼트 (선의 부드러움)
				FColor::Green, // 색상
				false, // 영구 여부 (false면 일정 시간 후 사라짐)
				2.0f, // 지속 시간
				0, // Depth Priority
				1.f // 선 두께
			);

			// Origin 위치 포인트로 표시
			DrawDebugPoint(
				World,
				SphereOrigin,
				10.0f,
				FColor::Yellow,
				false,
				1.0f
			);
		}

		TArray<FOverlapResult> OverlapResults;
		
		World->OverlapMultiByObjectType(
			OverlapResults,
			SphereOrigin,
			FQuat::Identity,
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),
			FCollisionShape::MakeSphere(Radius),
			SphereParams
		);
	
		for (FOverlapResult& Overlap : OverlapResults)
		{
			if (Overlap.GetActor()->Implements<UVNCombatInterface>() && !IVNCombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(Overlap.GetActor());
			}
		}
		
	}
}

void UVNMiniGameFunctionLibrary::GetHostileActorsWithinRadius(const UObject* WorldContextObject, const AActor* InstigatorActor,
	TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
	const FVector& SphereOrigin, bool bDrawDebugSphere)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);
	
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		if (bDrawDebugSphere)
		{
			// Origin 위치 메세지로 표시
			GEngine->AddOnScreenDebugMessage(
				-1,
				2.f,
				FColor::Cyan,
				FString::Printf(TEXT("SphereOrigin: %s"), *SphereOrigin.ToString())
			);

			// 범위 구형으로 표시
			DrawDebugSphere(
				World,
				SphereOrigin,
				Radius,
				16, // 세그먼트 (선의 부드러움)
				FColor::Green, // 색상
				false, // 영구 여부 (false면 일정 시간 후 사라짐)
				2.0f, // 지속 시간
				0, // Depth Priority
				1.f // 선 두께
			);

			// Origin 위치 포인트로 표시
			DrawDebugPoint(
				World,
				SphereOrigin,
				10.0f,
				FColor::Yellow,
				false,
				1.0f
			);
		}

		
		TArray<FOverlapResult> OverlapResults;
		
		World->OverlapMultiByObjectType(
			OverlapResults,
			SphereOrigin,
			FQuat::Identity,
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),
			FCollisionShape::MakeSphere(Radius),
			SphereParams
		);
	
		for (FOverlapResult& Overlap : OverlapResults)
		{
			if (!Overlap.GetActor()->Implements<UVNCombatInterface>() || IVNCombatInterface::Execute_IsDead(Overlap.GetActor())) continue;
			
			if (IsTargetPawnHostile(Cast<APawn>(Overlap.GetActor()), Cast<APawn>(InstigatorActor)))
			{
				OutOverlappingActors.AddUnique(Overlap.GetActor());
			}
		}
	}
}

bool UVNMiniGameFunctionLibrary::IsTargetPawnHostile(const APawn* QueryPawn, const APawn* TargetPawn)
{
	const IGenericTeamAgentInterface* QueryTeamAgent = Cast<IGenericTeamAgentInterface>(QueryPawn->GetController());
	const IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

	if (QueryTeamAgent && TargetTeamAgent)
	{
		return QueryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId(); 
	}

	return false;
}

AActor* UVNMiniGameFunctionLibrary::GetClosestActor(const TArray<AActor*>& TargetActors, const FVector& Origin)
{
	if (TargetActors.Num() == 0)
	{
		return nullptr;
	}
	
	AActor* ClosestTarget = nullptr;
	float MinDistSq = TNumericLimits<float>::Max();

	for (AActor* Target : TargetActors)
	{
		if (!IsValid(Target)) continue;

		float DistSq = FVector::DistSquared(Target->GetActorLocation(), Origin);

		if (DistSq < MinDistSq)
		{
			MinDistSq = DistSq;
			ClosestTarget = Target;
		}
	}

	return ClosestTarget;

}

FVector UVNMiniGameFunctionLibrary::GetClosestActorLocation(const TArray<AActor*>& TargetActors, const FVector& Origin)
{
	if (TargetActors.Num() == 0)
	{
		return FVector::ZeroVector;
	}
	
	if (const AActor* ClosestTarget = GetClosestActor(TargetActors, Origin))
	{
		return ClosestTarget->GetActorLocation();
	}

	return FVector::ZeroVector;
}

UVNObjectPoolManager* UVNMiniGameFunctionLibrary::GetPoolManger(const UObject* WorldContextObject)
{
	if (WorldContextObject == nullptr) return nullptr;

	const UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	if (World == nullptr) return nullptr;

	UGameInstance* GameInstance = World->GetGameInstance();
	if (GameInstance == nullptr) return nullptr;

	if (const IVNPoolProviderInterface* PoolProvider = Cast<IVNPoolProviderInterface>(GameInstance))
	{
		return PoolProvider->GetObjectPoolManager();
	}

	return nullptr;
	
}
