#include "AI/PatrolPath.h"
#include "Net/UnrealNetwork.h"

APatrolPath::APatrolPath()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;
}

//void APatrolPath::BeginPlay()
//{
//    Super::BeginPlay();
//}

AActor* APatrolPath::GetWaypoint(int32 Index) const
{
    if (Waypoints.IsValidIndex(Index))
    {
        return Waypoints[Index];
    }
    return nullptr;
}

int32 APatrolPath::Num() const
{
    return Waypoints.Num();
}

void APatrolPath::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(APatrolPath, Waypoints);
}
