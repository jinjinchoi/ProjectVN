// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VNMiniGameFunctionLibrary.generated.h"

class UVNObjectPoolManager;
/**
 * 
 */
UCLASS()
class VNMINIGAME_API UVNMiniGameFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	

public:
	UFUNCTION(BlueprintCallable, Category="MiniGameFunctionLibrary|Gameplay Mechanics")
	static void GetLiveActorWithinRadius(
		const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore,
		float Radius, const FVector& SphereOrigin, const bool bDrawDebugSphere = false);

	static void GetHostileActorsWithinRadius(const UObject* WorldContextObject, const AActor* InstigatorActor, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore,
		float Radius, const FVector& SphereOrigin, bool bDrawDebugSphere = false);

	static bool IsTargetPawnHostile(const APawn* QueryPawn, const APawn* TargetPawn);

	static AActor* GetClosestActor(const TArray<AActor*>& TargetActors, const FVector& Origin);
	static FVector GetClosestActorLocation(const TArray<AActor*>& TargetActors, const FVector& Origin);
	static UVNObjectPoolManager* GetPoolManger(const UObject* WorldContextObject);
};
