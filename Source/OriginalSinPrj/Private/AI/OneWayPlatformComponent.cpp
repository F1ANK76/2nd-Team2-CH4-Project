#include "AI/OneWayPlatformComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UOneWayPlatformComponent::UOneWayPlatformComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    bIsJumping = false;
    PreviousZVelocity = -1.0f;
    bWasOnGround = true;

    SetIsReplicatedByDefault(true);
}

void UOneWayPlatformComponent::BeginPlay()
{
    Super::BeginPlay();

    AActor* Owner = GetOwner();
    if (Owner)
    {
        ACharacter* CharacterOwner = Cast<ACharacter>(Owner);
        if (CharacterOwner)
        {
            MovementComp = CharacterOwner->GetCharacterMovement();
            CapsuleComp = CharacterOwner->GetCapsuleComponent();
        }
    }

    if (Owner && Owner->HasAuthority() && CapsuleComp)
    {
        CapsuleComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
    }
}

void UOneWayPlatformComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!MovementComp || !CapsuleComp)
    {
        return;
    }

    AActor* Owner = GetOwner();
    if (Owner && Owner->HasAuthority())
    {
        bool bIsOnGround = MovementComp->IsMovingOnGround();
        if (bWasOnGround && !bIsOnGround && MovementComp->Velocity.Z > 0.0f)
        {
            OnJumpStarted();
        }
        bWasOnGround = bIsOnGround;

        if (bIsJumping)
        {
            HandleJumpLogic(DeltaTime);
        }
    }
}

void UOneWayPlatformComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UOneWayPlatformComponent, bIsJumping);
}

void UOneWayPlatformComponent::OnRep_IsJumping()
{
    if (bIsJumping)
    {
        if (CapsuleComp)
        {
            CapsuleComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
        }
    }
    else
    {
        if (CapsuleComp)
        {
            CapsuleComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
        }
    }
}

bool UOneWayPlatformComponent::OnJumpStarted_Validate()
{
    return true;
}

void UOneWayPlatformComponent::OnJumpStarted_Implementation()
{
    AActor* Owner = GetOwner();
    if (Owner && Owner->HasAuthority() && MovementComp && CapsuleComp)
    {
        bIsJumping = true;
        CapsuleComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
        PreviousZVelocity = -1.0f;
    }
}

bool UOneWayPlatformComponent::OnJumpEnded_Validate()
{
    return true;
}

void UOneWayPlatformComponent::OnJumpEnded_Implementation()
{
    AActor* Owner = GetOwner();
    if (Owner && Owner->HasAuthority())
    {
        bIsJumping = false;
        if (CapsuleComp)
        {
            CapsuleComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
        }
    }
}

void UOneWayPlatformComponent::HandleJumpLogic(float DeltaTime)
{
    if (!MovementComp || !CapsuleComp)
    {
        return;
    }

    float CurrentZVelocity = MovementComp->Velocity.Z;

    if (PreviousZVelocity > 0.0f && CurrentZVelocity <= 0.0f)
    {
        OnJumpEnded();
    }
    else if (FMath::Abs(CurrentZVelocity) < 10.0f)
    {
        OnJumpEnded();
    }

    PreviousZVelocity = CurrentZVelocity;
}