// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/BTTask/BTTask_HijackPatternAttack.h"
#include "Boss/BossController.h"
#include "Boss/BossCharacter.h"
#include "Boss/BossObjectPoolWorldSubsystem.h"
#include "Boss/HijackBossCharacter.h"
#include "Boss/HijackBossController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

EBTNodeResult::Type UBTTask_HijackPatternAttack::ExecuteTask(UBehaviorTreeComponent& BTComponent, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Warning, TEXT("HijackPatternAttack Called"));
	BossController = Cast<ABossController>(BTComponent.GetAIOwner());
	if (!IsValid(BossController)) return EBTNodeResult::Failed;
	BossCharacter = Cast<ABossCharacter>(BossController->GetPawn());
	if (!IsValid(BossCharacter)) return EBTNodeResult::Failed;
	HijackBossCharacter = Cast<AHijackBossCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AHijackBossCharacter::StaticClass()));
	if (!IsValid(HijackBossCharacter)) return EBTNodeResult::Failed;
	HijackBossController = Cast<AHijackBossController>(HijackBossCharacter->GetController());
	if (!IsValid(HijackBossController)) return EBTNodeResult::Failed;
	PoolWorldSubsystem = GetWorld()->GetSubsystem<UBossObjectPoolWorldSubsystem>();
	if (!IsValid(PoolWorldSubsystem)) return EBTNodeResult::Failed;

	//납치 대상 플레이어 설정
	TargetPlayer = BossController->GetTargetPlayerPawn();
	
	//애니메이션
	BossCharacter->PlayHijackAttackMontage();
	
	//파괴가능 오브젝트들 전부 비활성화
	BossController->SetDestructibleObjectCount(0);
	TArray<AActor*> DestructibleObjects;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADestructibleObject::StaticClass(), DestructibleObjects);
	for (AActor* Object : DestructibleObjects)
	{
		PoolWorldSubsystem->ReturnActorToPool(Object);
	}

	//오브젝트 생성 타이머 비활성화
	BossController->DeactiveObjectSpawnTimer();

	//남은 플레이어가 파괴해야할 오브젝트 생성
	for (int i=0; i<BossController->GetMaxDestructibleObject(); ++i)
	{
		BossController->SpawnDestructibleObject();
	}

	// 플레이어 이동 시키고, 납치패턴전용 보스 활성화
	TargetPlayer->SetActorLocation(HijackBossController->GetPlayerSpawnLocation()); //임시
	HijackBossController->SwitchBattleState();
	bIsTaskExecuting = true;

	//플래그 설정
	BTComponent.GetBlackboardComponent()->SetValueAsBool("bIsHpUnder75AttackPlayed", true);
	BTComponent.GetBlackboardComponent()->SetValueAsBool("bTriggerHp50Attack", false);
	BTComponent.GetBlackboardComponent()->SetValueAsBool("bCanSpecialAttack", false);
	
	return EBTNodeResult::InProgress;
}

void UBTTask_HijackPatternAttack::TickTask(UBehaviorTreeComponent& BTComponent, uint8* NodeMemory, float DeltaSeconds)
{
	if (!bIsTaskExecuting)
	{
		HijackBossController->SwitchBattleState();
		
		//플레이어 귀환 로직
		TargetPlayer->SetActorLocation(BossCharacter->GetActorLocation() + FVector(0.0f, 2000.0f, 0.0f));
		
		//파괴가능 오브젝트 생성 타이머 활성화
		BossController->ActiveObjectSpawnTimer();
		
		FinishLatentTask(BTComponent, EBTNodeResult::Succeeded);
		return;
	}

	//플레이어 사망 여부, 남은 플레이어의 오브젝트 파괴 여부, 보스의 사망 여부
	bool bIsPlayerDead = false;
	bool bIsAllObjectDestroyed = BossController->GetDestructibleObjectCount() == 0 ? true : false;
	bool bIsBossDead = HijackBossCharacter->GetCurrentHP() <= 0 ? true : false;

	if (bIsBossDead || bIsAllObjectDestroyed || bIsPlayerDead)
	{
		bIsTaskExecuting = false;
	}
}
