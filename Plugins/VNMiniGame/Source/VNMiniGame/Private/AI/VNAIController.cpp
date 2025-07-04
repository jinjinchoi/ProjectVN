// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "AI/VNAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AVNAIController::AVNAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AISenseConfig_Sight"));
	AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
	AISenseConfig_Sight->SightRadius = 1500.f;
	AISenseConfig_Sight->LoseSightRadius = 2500.f;
	AISenseConfig_Sight->PeripheralVisionAngleDegrees = 180.f;

	AIPerceptionComponent->ConfigureSense(*AISenseConfig_Sight);
	AIPerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ThisClass::OnEnemyPerceptionUpdated);

	AAIController::SetGenericTeamId(FGenericTeamId(1));
	
}

ETeamAttitude::Type AVNAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* OtherPawn = Cast<const APawn>(&Other);
	if (!OtherPawn) return ETeamAttitude::Neutral;

	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(OtherPawn->GetController());
	if (!OtherTeamAgent) return ETeamAttitude::Neutral;

	const FGenericTeamId OtherTeamId = OtherTeamAgent->GetGenericTeamId();
	const FGenericTeamId MyTeamId = GetGenericTeamId();

	if (OtherTeamId == MyTeamId)
	{
		return ETeamAttitude::Friendly;
	}
	else
	{
		return ETeamAttitude::Hostile;
	}
	
}

void AVNAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	if (!BlackboardComponent) return;

	if (!IsValid(Actor))
	{
		BlackboardComponent->ClearValue("TargetActor");
		GetWorldTimerManager().ClearTimer(LostTargetTimer);
		return;
	}

	if (Stimulus.WasSuccessfullySensed())
	{
		GetWorld()->GetTimerManager().ClearTimer(LostTargetTimer);
		if (!BlackboardComponent->GetValueAsObject(FName("TargetActor")))
		{
			BlackboardComponent->SetValueAsObject(FName("TargetActor"), Actor);
		}
	}
	else
	{
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, FName("HandleLostTarget"), Actor);
		
		GetWorldTimerManager().SetTimer(LostTargetTimer, TimerDelegate, LostTargetDelay, false);
		
	}
	
}

void AVNAIController::HandleLostTarget(AActor* LostActor)
{
	if (!IsValid(LostActor)) return;
	
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	if (!BlackboardComponent) return;

	UObject* CurrentTarget = BlackboardComponent->GetValueAsObject("TargetActor");
	if (IsValid(CurrentTarget) && CurrentTarget == LostActor)
	{
		BlackboardComponent->ClearValue("TargetActor");
	}
	
}
