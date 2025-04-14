#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPath.generated.h"

UCLASS()
class ORIGINALSINPRJ_API APatrolPath : public AActor
{
    GENERATED_BODY()

public:
    APatrolPath();

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Patrol")
    TArray<AActor*> Waypoints;

    AActor* GetWaypoint(int32 Index) const;

    int32 Num() const;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

//protected:
    //virtual void BeginPlay() override;

};