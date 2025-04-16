// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/BTTask/BTTask_InstantDeathPatternAttack.h"
#include "Boss/BossController.h"
#include "Boss/BossCharacter.h"
#include "Boss/BossObjectPoolWorldSubsystem.h"
#include "Boss/PlatformSpawnTarget.h"
#include "Kismet/GameplayStatics.h"

EBTNodeResult::Type UBTTask_InstantDeathPatternAttack::ExecuteTask(UBehaviorTreeComponent& BTComponent,
                                                                   uint8* NodeMemory)
{
	BTComp = &BTComponent;
	BossController = Cast<ABossController>(BTComponent.GetAIOwner());
	if (!IsValid(BossController)) return EBTNodeResult::Failed;
	BossCharacter = Cast<ABossCharacter>(BossController->GetPawn());
	if (!IsValid(BossCharacter)) return EBTNodeResult::Failed;
	PoolWorldSubsystem = GetWorld()->GetSubsystem<UBossObjectPoolWorldSubsystem>();
	if (!IsValid(PoolWorldSubsystem)) return EBTNodeResult::Failed;
	PlatformSpawnTargets = BossController->GetPlatformSpawnTargets();
	if (PlatformSpawnTargets.Num() == 0) return EBTNodeResult::Failed;

	UAnimInstance* BossAnimInstance = BossCharacter->GetMesh()->GetAnimInstance();
	if (IsValid(BossCharacter->InstantDeathAttackMontage) && IsValid(BossAnimInstance))
	{
		if (!BossAnimInstance->OnMontageEnded.IsAlreadyBound(this, &UBTTask_InstantDeathPatternAttack::OnMontageEnded))
		{
			BossAnimInstance->OnMontageEnded.AddDynamic(this, &UBTTask_InstantDeathPatternAttack::OnMontageEnded);	
		}
	}
	BossCharacter->PlayInstantDeathAttackMontage();
	
	//파괴가능 오브젝트들 전부 비활성화
	BossController->SetDestructibleObjectCount(0);
	TArray<AActor*> DestructibleObjects;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADestructibleObject::StaticClass(), DestructibleObjects);
	for (AActor* Object : DestructibleObjects)
	{
		PoolWorldSubsystem->ReturnActorToPool(Object);
	}
	
	return EBTNodeResult::InProgress;
}

void UBTTask_InstantDeathPatternAttack::OnTaskFinished(UBehaviorTreeComponent& BTComponent, uint8* NodeMemory,
	EBTNodeResult::Type NodeResult)
{
	Super::OnTaskFinished(BTComponent, NodeMemory, NodeResult);
	
	if (IsValid(BossCharacter))
	{
		UAnimInstance* BossAnimInstance = BossCharacter->GetMesh()->GetAnimInstance();
		if (IsValid(BossAnimInstance))
		{
			BossAnimInstance->OnMontageEnded.RemoveDynamic(this, &UBTTask_InstantDeathPatternAttack::OnMontageEnded);
		}
	}
}

void UBTTask_InstantDeathPatternAttack::SpawnPlatform()
{
	TArray<APlatformSpawnTarget*> SpawnTargets;
	
	for (AActor* Actor : PlatformSpawnTargets)
	{
		APlatformSpawnTarget* SpawnTarget = Cast<APlatformSpawnTarget>(Actor);
		if (IsValid(SpawnTarget)) SpawnTargets.Add(SpawnTarget);
	}

	int32 MaxAttemps = 100;
	int32 AttempCount = 0;
	while (PlatformCount < MaxPlatform && AttempCount < MaxAttemps)
	{
		AttempCount++;
		int32 RandomIndex = FMath::RandRange(0, SpawnTargets.Num() - 1);

		if (!SpawnTargets[RandomIndex]->GetbIsUsed())
		{
			SpawnTargets[RandomIndex]->SetUsed();
			FVector SpawnLocation = SpawnTargets[RandomIndex]->GetActorLocation();
			FRotator SpawnRotation = SpawnTargets[RandomIndex]->GetActorRotation();

			PoolWorldSubsystem->SpawnBossPlatform(SpawnLocation, SpawnRotation);
			PlatformCount++;
		}
	}

	PlatformCount = 0;
	for (APlatformSpawnTarget* SpawnTarget : SpawnTargets)
	{
		SpawnTarget->SetUnUsed();
	}
}

void UBTTask_InstantDeathPatternAttack::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!IsValid(BossCharacter) || Montage != BossCharacter->InstantDeathAttackMontage) return;

	UAnimInstance* BossAnimInstance = BossCharacter->GetMesh()->GetAnimInstance();
	if (IsValid(BossAnimInstance))
	{
		BossAnimInstance->OnMontageEnded.RemoveDynamic(this, &UBTTask_InstantDeathPatternAttack::OnMontageEnded);
	}

	if (IsValid(BTComp) && !BTComp->HasAnyFlags(RF_BeginDestroyed | RF_FinishDestroyed))
	{
		SpawnPlatform();
		BossController->StartInstantDeathAttackTimer();
		FinishLatentTask(*BTComp, EBTNodeResult::Succeeded);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("RangeAttack : BTComp가 이미 Destroy됨"));
	}
}