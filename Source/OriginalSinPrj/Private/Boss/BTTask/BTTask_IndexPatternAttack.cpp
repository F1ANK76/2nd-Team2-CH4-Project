// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/BTTask/BTTask_IndexPatternAttack.h"
#include "Boss/BossController.h"
#include "Boss/BossCharacter.h"
#include "Boss/Object/IndexPatternProjectile.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Boss/BossObjectPoolWorldSubsystem.h"
#include "Kismet/GameplayStatics.h"

UBTTask_IndexPatternAttack::UBTTask_IndexPatternAttack()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_IndexPatternAttack::ExecuteTask(UBehaviorTreeComponent& BTComponent, uint8* NodeMemory)
{
	BossController = Cast<ABossController>(BTComponent.GetAIOwner());
	if (!IsValid(BossController)) return EBTNodeResult::Failed;
	BossCharacter = Cast<ABossCharacter>(BossController->GetPawn());
	if (!IsValid(BossCharacter)) return EBTNodeResult::Failed;
	PoolWorldSubsystem = GetWorld()->GetSubsystem<UBossObjectPoolWorldSubsystem>();

	BossCharacter->SetCurrentTaskNode(this);

	//파괴가능 오브젝트들 전부 비활성화
	BossController->SetDestructibleObjectCount(0);
	TArray<AActor*> DestructibleObjects;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADestructibleObject::StaticClass(), DestructibleObjects);
	for (AActor* Object : DestructibleObjects)
	{
		PoolWorldSubsystem->ReturnActorToPool(Object);
	}
	
	BossCharacter->PlayIndexAttackMontage();

	//플래그 설정
	BTComponent.GetBlackboardComponent()->SetValueAsBool("bIsHpUnder25AttackPlayed", true);
	BTComponent.GetBlackboardComponent()->SetValueAsBool("bTriggerHp25Attack", false);
	BTComponent.GetBlackboardComponent()->SetValueAsBool("bCanSpecialAttack", false);
	
	return EBTNodeResult::InProgress;
}

void UBTTask_IndexPatternAttack::TickTask(UBehaviorTreeComponent& BTComponent, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(BTComponent, NodeMemory, DeltaSeconds);

	if (!bIsExecuting) return;
	
	if (TimeElapsed >= Duration)
	{
		TimeElapsed = 0.0f;
		ShotCooldown = InitialShotCooldown;
		RotationOffset = 0.0f;

		UAnimInstance* BossAnimInstance = BossCharacter->GetMesh()->GetAnimInstance();
		BossAnimInstance->Montage_Stop(0.2f);

		FinishLatentTask(BTComponent, EBTNodeResult::Succeeded);
		return;
	}
	
	TimeElapsed += DeltaSeconds;
	ShotCooldown -= DeltaSeconds;
	RotationOffset += 40.0f * DeltaSeconds;

	if (ShotCooldown <= 0.0f)
	{
		ShotCooldown = InitialShotCooldown;
		for (int32 i = 0; i < 4; ++i)
		{
			float AngleDegrees = RotationOffset + i * 90.0f;
			float Radian = FMath::DegreesToRadians(AngleDegrees);

			FVector Direction = FVector(0.0f, FMath::Cos(Radian), FMath::Sin(Radian));
			FRotator SpawnRotation = Direction.Rotation();

			FVector SpawnLocation = BossCharacter->GetActorLocation();

			AIndexPatternProjectile* Projectile = PoolWorldSubsystem->SpawnIndexPatternProjectile(
				SpawnLocation, SpawnRotation);

			if (IsValid(Projectile))
			{
				Projectile->SetIndex(i);
				Projectile->SetDirectionAndVelocity(Direction);
			}
		}
	}
}