// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AnimNotify/AnimNotify_EndAttack.h"
#include "Player/WitchAbilityComponent.h"
#include "GameFramework/Character.h"

void UAnimNotify_EndAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	ACharacter* OwnerActor = Cast<ACharacter>(MeshComp->GetOwner());

	if (!IsValid(OwnerActor))
	{
		return;
	}

	if (MeshComp != OwnerActor->GetMesh())
	{
		return;
	}

	UWitchAbilityComponent* AbilityComp = OwnerActor->GetComponentByClass<UWitchAbilityComponent>();

	if (!IsValid(AbilityComp))
	{
		return;
	}

	AbilityComp->ResponseEndAttack();
}
