// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/AnimNotify/AnimNotify_BTTaskTrigger.h"
#include "Boss/BossCharacter.h"
#include "Boss/BTTask/BTTask_HijackPatternAttack.h"
#include "Boss/BTTask/BTTask_IndexPatternAttack.h"

void UAnimNotify_BTTaskTrigger::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!IsValid(MeshComp)) return;

	ABossCharacter* BossCharacter = Cast<ABossCharacter>(MeshComp->GetOwner());
	if (!IsValid(BossCharacter)) return;
	
	UBTTaskNode* CurrentTaskNode = BossCharacter->GetCurrentTaskNode();

	if (TaskName == FName("IndexAttack"))
	{
		if (UBTTask_IndexPatternAttack* IndexAttackTask = Cast<UBTTask_IndexPatternAttack>(CurrentTaskNode))
		{
			IndexAttackTask->SetIsExecuting(true);
		}
	}
	else if (TaskName == FName("HijackAttack"))
	{
		if (UBTTask_HijackPatternAttack* HijackAttackTask = Cast<UBTTask_HijackPatternAttack>(CurrentTaskNode))
		{
			HijackAttackTask->InitialTask();
		}
	}
}
