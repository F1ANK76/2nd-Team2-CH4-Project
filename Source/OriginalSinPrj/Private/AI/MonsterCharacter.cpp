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
#include "Player/BuffComponent.h"
#include "Engine/DamageEvents.h"

AMonsterCharacter::AMonsterCharacter()
{
    KnockbackIncreaseAmount = 15.0f;
    BaseKnockbackStrength = 2000.0f;
    KnockbackScaleFactor = 3.0f;
    bIsPlayerHit = false;
    bIsAttack = false;
    bIsDead = false;
    ExpValue = 50;

    BuffComp = CreateDefaultSubobject<UBuffComponent>(TEXT("Buff Component"));

    AIControllerClass = AEnemyAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    AttackCollider = CreateDefaultSubobject<USphereComponent>(TEXT("AttackCollider"));
    AttackCollider->SetupAttachment(GetMesh(), FName("hand_r"));
    AttackCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    AttackCollider->OnComponentBeginOverlap.AddDynamic(this, &AMonsterCharacter::OnAttackOverlap);

    UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
    if (MovementComponent)
    {
        MovementComponent->bConstrainToPlane = false;
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
    DOREPLIFETIME(AMonsterCharacter, KnockbackGauge);
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

                KnockbackGauge = FMath::Clamp(KnockbackGauge + KnockbackIncreaseAmount, 0.0f, MaxKnockbackGauge);

                ApplyAttack(OtherActor, Damage);

                UKnockbackComponent* PlayerKnockbackComp = Player->FindComponentByClass<UKnockbackComponent>();
                if (PlayerKnockbackComp)
                {
                    PlayerKnockbackComp->AddKnockbackGauge(KnockbackIncreaseAmount); //플레이어 넉백 게이지

                    float KnockbackStrength = BaseKnockbackStrength + (KnockbackGauge * KnockbackScaleFactor);
                    FVector KnockbackDirection = (Player->GetActorLocation() - GetActorLocation()).GetSafeNormal();
                    PlayerKnockbackComp->ApplyKnockback(KnockbackDirection, KnockbackStrength);
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
            // Player->AddExp(ExpValue);
        }
    }
}

void AMonsterCharacter::ApplyAttack(AActor* Target, float ApplyValue)
{
    if (!IsValid(Target))
    {
        return;
    }

    AMonsterCharacter* MonsterTarget = Cast<AMonsterCharacter>(Target);
    if (MonsterTarget)
    {
        return;
    }

    float RealDamage = ApplyValue + AddedKnockGauge;
    Target->TakeDamage(RealDamage, FDamageEvent(), GetController(), this);
}

float AMonsterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    if (!HasAuthority() || !IsValid(DamageCauser) || bIsDead)
    {
        return 0.0f;
    }

    AMonsterCharacter* MonsterCauser = Cast<AMonsterCharacter>(DamageCauser);
    if (MonsterCauser)
    {
        return 0.0f;
    }

    ABaseWitch* CauserWitch = Cast<ABaseWitch>(DamageCauser);
    float DecreaseValue = 1.0f;
    float RealDamage = FMath::Clamp(DamageAmount - DecreaseValue, 0.0f, 100.0f);

    // KnockbackGauge 증가
    KnockbackGauge = FMath::Clamp(KnockbackGauge + (RealDamage * 1.3), 0.0f, MaxKnockbackGauge);

    UKnockbackComponent* MonsterKnockbackComp = FindComponentByClass<UKnockbackComponent>();
    if (MonsterKnockbackComp)
    {
        float KnockbackStrength = BaseKnockbackStrength + (KnockbackGauge * KnockbackScaleFactor);
        FVector KnockbackDirection = (GetActorLocation() - DamageCauser->GetActorLocation()).GetSafeNormal();
        MonsterKnockbackComp->ApplyKnockback(KnockbackDirection, KnockbackStrength);
    }
    return RealDamage;
}