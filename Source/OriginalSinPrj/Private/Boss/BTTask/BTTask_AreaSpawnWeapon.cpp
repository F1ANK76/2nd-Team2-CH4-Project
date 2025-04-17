// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/BTTask/BTTask_AreaSpawnWeapon.h"

#include "Boss/BossCharacter.h"
#include "Boss/BossController.h"
#include "Boss/BossObjectPoolWorldSubsystem.h"
#include "Boss/Object/WeaponToSpawn.h"
#include "GameState/BaseGameState.h"
#include "OriginalSinPrj/GameInstance/EnumSet.h"

EBTNodeResult::Type UBTTask_AreaSpawnWeapon::ExecuteTask(UBehaviorTreeComponent& BTComponent, uint8* NodeMemory)
{
	BTComp = &BTComponent;
	BossController = Cast<ABossController>(BTComponent.GetAIOwner());
	if (!IsValid(BossController)) return EBTNodeResult::Failed;
	BossCharacter = Cast<ABossCharacter>(BossController->GetPawn());
	if (!IsValid(BossCharacter)) return EBTNodeResult::Failed;

	UAnimInstance* BossAnimInstance = BossCharacter->GetMesh()->GetAnimInstance();
	if (IsValid(BossCharacter->AreaSpawnWeaponMontage) && IsValid(BossAnimInstance))
	{
		if (!BossAnimInstance->OnMontageEnded.IsAlreadyBound(this, &UBTTask_AreaSpawnWeapon::OnMontageEnded))
		{
			BossAnimInstance->OnMontageEnded.AddDynamic(this, &UBTTask_AreaSpawnWeapon::OnMontageEnded);	
		}
	}
	BossCharacter->PlayAreaSpawnWeaponMontage();

	ABaseGameState* GameState = GetWorld()->GetGameState<ABaseGameState>();
	if (IsValid(GameState))
	{
		GameState->PlayBossSound(BossCharacter->GetAudioComponent(), EBossSoundType::SpawnWeapon);
	}
	
	return EBTNodeResult::InProgress;
}

void UBTTask_AreaSpawnWeapon::OnTaskFinished(UBehaviorTreeComponent& BTComponent, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(BTComponent, NodeMemory, TaskResult);

	if (IsValid(BossCharacter))
	{
		UAnimInstance* BossAnimInstance = BossCharacter->GetMesh()->GetAnimInstance();
		if (IsValid(BossAnimInstance))
		{
			BossAnimInstance->OnMontageEnded.RemoveDynamic(this, &UBTTask_AreaSpawnWeapon::OnMontageEnded);
		}
	}
}

void UBTTask_AreaSpawnWeapon::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!IsValid(BossCharacter) || Montage != BossCharacter->AreaSpawnWeaponMontage) return;

	UAnimInstance* BossAnimInstance = BossCharacter->GetMesh()->GetAnimInstance();
	if (IsValid(BossAnimInstance))
	{
		BossAnimInstance->OnMontageEnded.RemoveDynamic(this, &UBTTask_AreaSpawnWeapon::OnMontageEnded);
	}

	if (IsValid(BTComp) && !BTComp->HasAnyFlags(RF_BeginDestroyed | RF_FinishDestroyed))
	{
		BossController->StartSpawnWeaponAttack();
		FinishLatentTask(*BTComp, EBTNodeResult::Succeeded);
	}
}
