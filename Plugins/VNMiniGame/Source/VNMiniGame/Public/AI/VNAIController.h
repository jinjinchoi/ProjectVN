// Copyright © 2025 Choi-JinWoo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "VNAIController.generated.h"

class UAISenseConfig_Sight;
/**
 * 
 */
UCLASS()
class VNMINIGAME_API AVNAIController : public AAIController
{
	GENERATED_BODY()

public:
	AVNAIController(const FObjectInitializer& ObjectInitializer);

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "AI")
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	TObjectPtr<UAISenseConfig_Sight> AISenseConfig_Sight;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float LostTargetDelay = 5.f;

private:
	UFUNCTION()
	void OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()
	void HandleLostTarget(AActor* LostActor);

	FTimerHandle LostTargetTimer;
	
};
