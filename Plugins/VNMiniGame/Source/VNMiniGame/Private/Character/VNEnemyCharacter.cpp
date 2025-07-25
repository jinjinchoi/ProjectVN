// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Character/VNEnemyCharacter.h"

#include "VNMiniGameFunctionLibrary.h"
#include "AI/VNAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Component/Combat/VNAbilityComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Manager/Pooling/VNObjectPoolManager.h"
#include "UI/Widget/VNMiniGameUserWidget.h"


AVNEnemyCharacter::AVNEnemyCharacter()
{
	AutoPossessAI = EAutoPossessAI::Disabled;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 180.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	
	HealthBarComponent = CreateDefaultSubobject<UWidgetComponent>("HealthBar Component");
	HealthBarComponent->SetupAttachment(GetRootComponent());
	
}

void AVNEnemyCharacter::OnCharacterHit_Implementation(bool InbIsHit)
{
	Super::OnCharacterHit_Implementation(InbIsHit);

	if (AVNAIController* AIController = Cast<AVNAIController>(GetController()))
	{
		if (UBlackboardComponent* BB = AIController->GetBlackboardComponent())
		{
			BB->SetValueAsBool("IsHit", bIsHit);
		}
	}
	
}

void AVNEnemyCharacter::OnCharacterDeath_Implementation()
{
	Super::OnCharacterDeath_Implementation();
	
	if (AController* AIController = GetController())
	{
		AIController->UnPossess();
	}

	TWeakObjectPtr<AActor> WeakThis = this;
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([WeakThis]()
	{
		if (!WeakThis.IsValid()) return;
		
		if (UVNObjectPoolManager* PoolManager = UVNMiniGameFunctionLibrary::GetPoolManger(WeakThis.Get()))
		{
			PoolManager->ReturnObject(WeakThis.Get());
		}
	}), DeathWaiteDuration, false);
	

}

void AVNEnemyCharacter::RequestUseAbility_Implementation(const EVNAbilityType InAbilityType)
{
	if (InAbilityType == EVNAbilityType::None)
	{
		return;
	}

	AbilityComponent->TryActivateAbilityByType(InAbilityType);
	
}

AActor* AVNEnemyCharacter::GetActorFromBlackboard(const FName& KeyName) const
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController && AIController->GetBlackboardComponent())
	{
		return Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(KeyName));
	}

	return nullptr;
}

void AVNEnemyCharacter::OnActivateFromPool_Implementation()
{
	if (bIsDead)
	{
		ReviveCharacter();
	}
	
}

void AVNEnemyCharacter::OnDeactivateToPool_Implementation()
{
	
}

void AVNEnemyCharacter::OnCharacterLaunched()
{
	Super::OnCharacterLaunched();

	if (AVNAIController* AIController = Cast<AVNAIController>(GetController()))
	{
		if (UBlackboardComponent* BB = AIController->GetBlackboardComponent())
		{
			BB->SetValueAsBool("IsKnockdown", bIsKnockdown);
		}
	}
	
}

void AVNEnemyCharacter::OnCharacterGotUp()
{
	Super::OnCharacterGotUp();

	if (AVNAIController* AIController = Cast<AVNAIController>(GetController()))
	{
		if (UBlackboardComponent* BB = AIController->GetBlackboardComponent())
		{
			BB->SetValueAsBool("IsKnockdown", bIsKnockdown);
		}
	}
	
}

void AVNEnemyCharacter::SpawnEnemy(const FTransform& SpawnTransform)
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	
	if (CachedController)
	{
		CachedController->Possess(this);
	}
	else
	{
		SpawnDefaultController();
		CachedController = GetController();
	}

	SetActorTransform(SpawnTransform);
	
}

void AVNEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("Enemy"));

	if (UVNMiniGameUserWidget* MiniGameWidget = Cast<UVNMiniGameUserWidget>(HealthBarComponent->GetUserWidgetObject()))
	{
		MiniGameWidget->SetWidgetController(this);
	}
	
}
