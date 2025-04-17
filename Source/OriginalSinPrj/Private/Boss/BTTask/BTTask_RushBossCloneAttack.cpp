// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/BTTask/BTTask_RushBossCloneAttack.h"

#include "Boss/BossCharacter.h"
#include "Boss/BossController.h"
#include "Boss/BossObjectPoolWorldSubsystem.h"
#include "Boss/Object/RushBossClone.h"

UBTTask_RushBossCloneAttack::UBTTask_RushBossCloneAttack()
{
	SpawnOffset = 100.0f;
}

EBTNodeResult::Type UBTTask_RushBossCloneAttack::ExecuteTask(UBehaviorTreeComponent& BTComponent, uint8* NodeMemory)
{
	BTComp = &BTComponent;
	BossController = Cast<ABossController>(BTComponent.GetAIOwner());
	if (!IsValid(BossController)) return EBTNodeResult::Failed;
	BossCharacter = Cast<ABossCharacter>(BossController->GetPawn());
	if (!IsValid(BossCharacter)) return EBTNodeResult::Failed;

	UAnimInstance* BossAnimInstance = BossCharacter->GetMesh()->GetAnimInstance();
	if (IsValid(BossCharacter->RushBossAttackMontage) && IsValid(BossAnimInstance))
	{
		if (!BossAnimInstance->OnMontageEnded.IsAlreadyBound(this, &UBTTask_RushBossCloneAttack::OnMontageEnded))
		{
			BossAnimInstance->OnMontageEnded.AddDynamic(this, &UBTTask_RushBossCloneAttack::OnMontageEnded);	
		}
	}
	BossCharacter->PlayRushBossAttackMontage();

	return EBTNodeResult::InProgress;
}

void UBTTask_RushBossCloneAttack::OnTaskFinished(UBehaviorTreeComponent& BTComponent, uint8* NodeMemory,
	EBTNodeResult::Type NodeResult)
{
	Super::OnTaskFinished(BTComponent, NodeMemory, NodeResult);

	if (IsValid(BossCharacter))
	{
		UAnimInstance* BossAnimInstance = BossCharacter->GetMesh()->GetAnimInstance();
		if (IsValid(BossAnimInstance))
		{
			BossAnimInstance->OnMontageEnded.RemoveDynamic(this, &UBTTask_RushBossCloneAttack::OnMontageEnded);
		}
	}
}

void UBTTask_RushBossCloneAttack::CloningRushBoss()
{
	FVector SpawnLocation = BossCharacter->GetActorLocation();
	SpawnLocation.Y += SpawnLocation.Y < 0 ? -SpawnOffset : SpawnOffset;
	
	UBossObjectPoolWorldSubsystem* PoolWorldSubsystem = GetWorld()->GetSubsystem<UBossObjectPoolWorldSubsystem>();
	if (IsValid(PoolWorldSubsystem))
	{
		ARushBossClone* RushBossClone = PoolWorldSubsystem->SpawnRushBossClone(SpawnLocation, FRotator::ZeroRotator);
	}
}

void UBTTask_RushBossCloneAttack::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!IsValid(BossCharacter) || Montage != BossCharacter->RushBossAttackMontage) return;

	UAnimInstance* BossAnimInstance = BossCharacter->GetMesh()->GetAnimInstance();
	if (IsValid(BossAnimInstance))
	{
		BossAnimInstance->OnMontageEnded.RemoveDynamic(this, &UBTTask_RushBossCloneAttack::OnMontageEnded);
	}

	if (IsValid(BTComp) && !BTComp->HasAnyFlags(RF_BeginDestroyed | RF_FinishDestroyed))
	{
		CloningRushBoss();
		FinishLatentTask(*BTComp, EBTNodeResult::Succeeded);
	}
}
