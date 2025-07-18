// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Actor/DialougeSystem/VNDialogueVolume.h"

#include "Components/BoxComponent.h"
#include "Interface/VNDialogueControllerInterface.h"
#include "Interface/VNPlayerInterface.h"

AVNDialogueVolume::AVNDialogueVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	BoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SetRootComponent(BoxCollision);

}

void AVNDialogueVolume::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnSphereBeginOverlap);
	
}

void AVNDialogueVolume::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->Implements<UVNPlayerInterface>()) return;

	BoxCollision->OnComponentBeginOverlap.RemoveAll(this);

	if (DialogueNodeName == NAME_None) return;

	if (IVNDialogueControllerInterface* DialogueControllerInterface = Cast<IVNDialogueControllerInterface>(OtherActor->GetInstigatorController()))
	{
		DialogueControllerInterface->StartDialogue(DialogueNodeName);
	}
	
}
