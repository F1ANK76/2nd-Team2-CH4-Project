#include "AI/Player_AIController.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Player/BaseWitch.h"
#include "Net/UnrealNetwork.h"

void APlayer_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// 컨트롤러가 캐릭터에 빙의했을 때 호출
	ABaseWitch* ControlledPlayerAI = Cast<ABaseWitch>(InPawn);
	if (ControlledPlayerAI)
	{
		// CapsuleComp 초기화
		CapsuleComp = ControlledPlayerAI->GetCapsuleComponent();
	}

	SetReplicates(true);
}

void APlayer_AIController::BeginPlay()
{
	Super::BeginPlay();
}

void APlayer_AIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
}

bool APlayer_AIController::MoveToTargetWithJump_Validate(AActor* TargetActor)
{
	return true;
}

void APlayer_AIController::MoveToTargetWithJump_Implementation(AActor* TargetActor)
{
	ABaseWitch* ControlledPlayerAI = Cast<ABaseWitch>(GetPawn());
	if (!ControlledPlayerAI || !TargetActor)
	{
		return;
	}

	if (HasAuthority())
	{
		//ControlledPlayerAI->JumpToTargetPlatform(TargetActor);

		FVector TargetLocation = TargetActor->GetActorLocation();
		TargetLocation.X = ControlledPlayerAI->GetActorLocation().X; // X축 고정
		MoveToLocation(TargetLocation);
	}
}

bool APlayer_AIController::MoveToTargetWithFalling_Validate()
{
	return true;
}

void APlayer_AIController::MoveToTargetWithFalling_Implementation()
{
	ABaseWitch* ControlledPlayerAI = Cast<ABaseWitch>(GetPawn());
	if (!ControlledPlayerAI || !CapsuleComp)
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