#include "AI/KnockbackComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

UKnockbackComponent::UKnockbackComponent()
{
    KnockbackGauge = 0.0f;
    MaxKnockbackGauge = 200.0f;
    MinZKnockback = 0.5f;
    ZKnockbackStrengthMultiplier = 0.0015f;

    SetIsReplicatedByDefault(true);
}

void UKnockbackComponent::BeginPlay()
{
    Super::BeginPlay();

    //KnockbackGauge = 0.0f;
}

void UKnockbackComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UKnockbackComponent, KnockbackGauge);
}

bool UKnockbackComponent::AddKnockbackGauge_Validate(float Amount)
{
    return true;
}

void UKnockbackComponent::AddKnockbackGauge_Implementation(float Amount)
{
    if (GetOwner() && GetOwner()->HasAuthority())
    {
        KnockbackGauge = FMath::Clamp(KnockbackGauge + Amount, 0.0f, MaxKnockbackGauge);
    }
}

bool UKnockbackComponent::ApplyKnockback_Validate(FVector Direction, float BaseStrength)
{
    return true;
}

void UKnockbackComponent::ApplyKnockback_Implementation(FVector Direction, float BaseStrength)
{
    if (GetOwner() && GetOwner()->HasAuthority())
    {
        ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
        if (!OwnerCharacter)
        {
            return;
        }

        float KnockbackStrength = BaseStrength + (KnockbackGauge / MaxKnockbackGauge) * BaseStrength;

        float ZKnockback = MinZKnockback + (KnockbackStrength * ZKnockbackStrengthMultiplier);

        Direction.X = 0.0f;
        Direction.Y *= 1.5f;
        Direction.Z = FMath::Max(Direction.Z, ZKnockback);
        Direction.Normalize();

        OwnerCharacter->LaunchCharacter(Direction * KnockbackStrength, true, true);
    }
}

void UKnockbackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}