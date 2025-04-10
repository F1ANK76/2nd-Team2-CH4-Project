// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AnimNotify/AnimNotify_EndAttack.h"
#include "Player/BaseWitch.h"
#include "GameFramework/Character.h"

void UAnimNotify_EndAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	ABaseWitch* OwnerActor = Cast<ABaseWitch>(MeshComp->GetOwner());

	if (!IsValid(OwnerActor))
	{
		return;
	}

	if (MeshComp != OwnerActor->GetMesh())
	{
		return;
	}

	OwnerActor->EndAnimNotify();
}
