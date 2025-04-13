#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

UCLASS()
class ORIGINALSINPRJ_API AEnemyAIController : public AAIController
{
    GENERATED_BODY()

public:
    virtual void OnPossess(APawn* InPawn) override;
    virtual void BeginPlay() override;
    virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

    UFUNCTION(BlueprintCallable, Category = "AI", Server, Reliable, WithValidation)
    void MoveToTargetWithJump(AActor* TargetActor);

    bool MoveToTargetWithJump_Validate(AActor* TargetActor);
    void MoveToTargetWithJump_Implementation(AActor* TargetActor);

    UFUNCTION(BlueprintCallable, Category = "AI", Server, Reliable, WithValidation)
    void MoveToTargetWithFalling();

    bool MoveToTargetWithFalling_Validate();
    void MoveToTargetWithFalling_Implementation();

protected:

    // 딜레이 후 호출될 함수
   // void SetCollisionToBlock();

private:
    FTimerHandle TimerHandle;
    class UCapsuleComponent* CapsuleComp;

};
