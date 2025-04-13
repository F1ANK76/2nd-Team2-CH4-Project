//#include "AI/MonsterCharacter.h"
//#include "AI/EnemyAIController.h"
//#include "Kismet/GameplayStatics.h"
//#include "GameFramework/Character.h"
//#include "GameFramework/DamageType.h"
//#include "AI/KnockbackComponent.h"
//#include "Components/SphereComponent.h"
//#include "GameFramework/CharacterMovementComponent.h"
//#include "Engine/EngineTypes.h"
//#include "Components/CapsuleComponent.h"
//#include "Net/UnrealNetwork.h"
////#include "Engine/Engine.h"
//
//AMonsterCharacter::AMonsterCharacter()
//{
//    KnockbackIncreaseAmount = 10.0f;
//    BaseKnockbackStrength = 500.0f;
//    bIsPlayerHit = false;
//    bIsAttack = false; // 초기화
//
//    KnockbackComp = CreateDefaultSubobject<UKnockbackComponent>(TEXT("KnockbackComp"));
//    if (KnockbackComp)
//    {
//        KnockbackComp->KnockbackGauge = 70.0f;
//    }
//
//    AIControllerClass = AEnemyAIController::StaticClass();
//    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
//
//    AttackCollider = CreateDefaultSubobject<USphereComponent>(TEXT("AttackCollider"));
//    AttackCollider->SetupAttachment(GetMesh(), FName("hand_r"));
//    AttackCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//
//    AttackCollider->OnComponentBeginOverlap.AddDynamic(this, &AMonsterCharacter::OnAttackOverlap);
//
//    UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
//    if (MovementComponent)
//    {
//        MovementComponent->bConstrainToPlane = true;
//        MovementComponent->SetPlaneConstraintNormal(FVector(1.0f, 0.0f, 0.0f));
//        MovementComponent->bSnapToPlaneAtStart = true;
//        MovementComponent->JumpZVelocity = 800.0f;
//        MovementComponent->GravityScale = 1.0f; // 중력 활성화하기
//    }
//
//    UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
//    if (CapsuleComp)
//    {
//        CapsuleComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
//    }
//
//    // 레플리케이션 활성화
//    bReplicates = true; // 네트워크 레플리케이트 지원 설정임
//    bAlwaysRelevant = true; // 항상 네트워크와 관련 있음
//    SetReplicateMovement(true); // 이동 레플리케이션 활성화
//
//    // 초기 상태 설정
//    bIsJumping = false;
//    bIsMovingToBackLocation = false;
//}
//
//void AMonsterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//    // 레플리케이션할 변수 등록
//    DOREPLIFETIME(AMonsterCharacter, bIsJumping);
//    DOREPLIFETIME(AMonsterCharacter, bIsMovingToBackLocation);
//    DOREPLIFETIME(AMonsterCharacter, bIsPlayerHit);
//    DOREPLIFETIME(AMonsterCharacter, bIsAttack);
//}
//
//void AMonsterCharacter::JumpToTargetPlatform(AActor* TargetActor)
//{
//    if (!TargetActor)
//    {
//        return;
//    }
//
//    bIsJumping = true; // 점프 상태 업데이트 (레플리케이션됨)
//    // 발판 감지 로직 제거: 단순히 점프만 실행
//
//    Jump();
//
//    FVector Direction = (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
//    Direction.X = 0.0f;
//    Direction.Z = 0.0f;
//    AddMovementInput(Direction);
//}
//
//void AMonsterCharacter::OnAttackOverlap(
//    UPrimitiveComponent* OverlappedComponent,
//    AActor* OtherActor,
//    UPrimitiveComponent* OtherComp,
//    int32 OtherBodyIndex,
//    bool bFromSweep,
//    const FHitResult& SweepResult)
//{
//    if (HasAuthority())
//    {
//        if (OtherActor && OtherActor != this)
//        {
//            AMonsterCharacter* OtherMonster = Cast<AMonsterCharacter>(OtherActor);
//            if (OtherMonster)
//            {
//                return;
//            }
//
//            ACharacter* Player = Cast<ACharacter>(OtherActor);
//            if (Player && !bIsPlayerHit)
//            {
//                bIsPlayerHit = true;
//
//                UKnockbackComponent* PlayerKnockbackComp = Player->FindComponentByClass<UKnockbackComponent>();
//                if (PlayerKnockbackComp)
//                {
//                    PlayerKnockbackComp->AddKnockbackGauge(KnockbackIncreaseAmount);
//                    FVector KnockbackDirection = (Player->GetActorLocation() - GetActorLocation()).GetSafeNormal();
//                    PlayerKnockbackComp->ApplyKnockback(KnockbackDirection, BaseKnockbackStrength);
//                }
//            }
//        }
//    }
//}
//
//void AMonsterCharacter::EnableAttackCollider()
//{
//    AttackCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
//    bIsPlayerHit = false;
//}
//
//void AMonsterCharacter::DisableAttackCollider()
//{
//    AttackCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//}

#include "AI/MonsterCharacter.h"
#include "AI/EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/DamageType.h"
#include "AI/KnockbackComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/EngineTypes.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "Player/BaseWitch.h"

AMonsterCharacter::AMonsterCharacter()
{
    KnockbackIncreaseAmount = 10.0f;
    BaseKnockbackStrength = 500.0f;
    bIsPlayerHit = false;
    bIsAttack = false;
    bIsDead = false;
    ExpValue = 50;

    KnockbackComp = CreateDefaultSubobject<UKnockbackComponent>(TEXT("KnockbackComp"));
    if (KnockbackComp)
    {
        KnockbackComp->KnockbackGauge = 70.0f;
    }

    AIControllerClass = AEnemyAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    AttackCollider = CreateDefaultSubobject<USphereComponent>(TEXT("AttackCollider"));
    AttackCollider->SetupAttachment(GetMesh(), FName("hand_r"));
    AttackCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    AttackCollider->OnComponentBeginOverlap.AddDynamic(this, &AMonsterCharacter::OnAttackOverlap);

    UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
    if (MovementComponent)
    {
        MovementComponent->bConstrainToPlane = true;
        MovementComponent->SetPlaneConstraintNormal(FVector(1.0f, 0.0f, 0.0f));
        MovementComponent->bSnapToPlaneAtStart = true;
        MovementComponent->JumpZVelocity = 800.0f;
        MovementComponent->GravityScale = 1.0f;
    }

    UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
    if (CapsuleComp)
    {
        CapsuleComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
    }

    bReplicates = true;
    bAlwaysRelevant = true;
    SetReplicateMovement(true);

    bIsJumping = false;
    bIsMovingToBackLocation = false;
}

void AMonsterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AMonsterCharacter, bIsJumping);
    DOREPLIFETIME(AMonsterCharacter, bIsMovingToBackLocation);
    DOREPLIFETIME(AMonsterCharacter, bIsPlayerHit);
    DOREPLIFETIME(AMonsterCharacter, bIsAttack);
    DOREPLIFETIME(AMonsterCharacter, bIsDead);
}

void AMonsterCharacter::JumpToTargetPlatform(AActor* TargetActor)
{
    if (!TargetActor || bIsDead)
    {
        return;
    }

    bIsJumping = true;
    Jump();

    FVector Direction = (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
    Direction.X = 0.0f;
    Direction.Z = 0.0f;
    AddMovementInput(Direction);
}

void AMonsterCharacter::OnAttackOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (HasAuthority() && !bIsDead)
    {
        if (OtherActor && OtherActor != this)
        {
            AMonsterCharacter* OtherMonster = Cast<AMonsterCharacter>(OtherActor);
            if (OtherMonster)
            {
                return;
            }

            ACharacter* Player = Cast<ACharacter>(OtherActor);
            if (Player && !bIsPlayerHit)
            {
                bIsPlayerHit = true;

                UKnockbackComponent* PlayerKnockbackComp = Player->FindComponentByClass<UKnockbackComponent>();
                if (PlayerKnockbackComp)
                {
                    PlayerKnockbackComp->AddKnockbackGauge(KnockbackIncreaseAmount);
                    FVector KnockbackDirection = (Player->GetActorLocation() - GetActorLocation()).GetSafeNormal();
                    PlayerKnockbackComp->ApplyKnockback(KnockbackDirection, BaseKnockbackStrength);
                }
            }
        }
    }
}

void AMonsterCharacter::EnableAttackCollider()
{
    if (!bIsDead)
    {
        AttackCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        bIsPlayerHit = false;
    }
}

void AMonsterCharacter::DisableAttackCollider()
{
    AttackCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMonsterCharacter::ServerDie_Implementation()
{
    if (HasAuthority() && !bIsDead)
    {
        bIsDead = true;
        MulticastDie();

        ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
        if (Player)
        {
            ServerGiveExpToPlayer(Player);
        }

        DeactivateMonster();
    }
}

void AMonsterCharacter::MulticastDie_Implementation()
{
    bIsDead = true;
    DisableAttackCollider();
}

void AMonsterCharacter::DeactivateMonster()
{
    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);
    SetActorTickEnabled(false);
    if (AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController()))
    {
        AIController->SetActorTickEnabled(false);
    }
}

void AMonsterCharacter::ActivateMonster()
{
    bIsDead = false;
    bIsPlayerHit = false;
    bIsAttack = false;
    SetActorHiddenInGame(false);
    SetActorEnableCollision(true);
    SetActorTickEnabled(true);
    if (AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController()))
    {
        AIController->SetActorTickEnabled(true);
        AIController->Reset();
    }
}

void AMonsterCharacter::ServerGiveExpToPlayer_Implementation(ACharacter* PlayerCharacter)
{
    if (HasAuthority() && PlayerCharacter)
    {
        ABaseWitch* Player = Cast<ABaseWitch>(PlayerCharacter);
        if (Player)
        {
            //Player->AddExp(ExpValue); // basewitch에 AddExp 함수 만들기
        }
    }
}