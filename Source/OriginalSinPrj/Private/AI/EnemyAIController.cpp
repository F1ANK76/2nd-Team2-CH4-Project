#include "AI/EnemyAIController.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "AI/MonsterCharacter.h"
#include "Net/UnrealNetwork.h"

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// ��Ʈ�ѷ��� ĳ���Ϳ� �������� �� ȣ��
	AMonsterCharacter* ControlledMonster = Cast<AMonsterCharacter>(InPawn);
	if (ControlledMonster)
	{
		// CapsuleComp �ʱ�ȭ
		CapsuleComp = ControlledMonster->GetCapsuleComponent();
	}

	SetReplicates(true);//���ø�

}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
}

bool AEnemyAIController::MoveToTargetWithJump_Validate(AActor* TargetActor)
{
	return true;
}

void AEnemyAIController::MoveToTargetWithJump_Implementation(AActor* TargetActor)
{
	AMonsterCharacter* ControlledMonster = Cast<AMonsterCharacter>(GetPawn());
	if (!ControlledMonster || !TargetActor)
	{
		return;
	}

	if (HasAuthority())
	{
		ControlledMonster->JumpToTargetPlatform(TargetActor);

		FVector TargetLocation = TargetActor->GetActorLocation();
		TargetLocation.X = ControlledMonster->GetActorLocation().X; // X�� ����
		MoveToLocation(TargetLocation);
	}
}

bool AEnemyAIController::MoveToTargetWithFalling_Validate()
{
	return true;
}

void AEnemyAIController::MoveToTargetWithFalling_Implementation()
{
	AMonsterCharacter* ControlledMonster = Cast<AMonsterCharacter>(GetPawn());
	if (!ControlledMonster || !CapsuleComp)
	{
		return;
	}

	if (HasAuthority())
	{
		CapsuleComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);

		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			[this]()
		{
			if (CapsuleComp)
			{
				CapsuleComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
			}
		},
			0.5f,
			false
			
		);
	}
}