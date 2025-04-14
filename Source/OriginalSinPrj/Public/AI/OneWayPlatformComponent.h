#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "OneWayPlatformComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ORIGINALSINPRJ_API UOneWayPlatformComponent : public USceneComponent
{
    GENERATED_BODY()

public:
    UOneWayPlatformComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category = "OneWayPlatform", Server, Reliable, WithValidation)
    void OnJumpStarted();

    bool OnJumpStarted_Validate();
    void OnJumpStarted_Implementation();

    UFUNCTION(BlueprintCallable, Category = "OneWayPlatform", Server, Reliable, WithValidation)
    void OnJumpEnded();

    bool OnJumpEnded_Validate();
    void OnJumpEnded_Implementation();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
    UPROPERTY(Transient)
    UCharacterMovementComponent* MovementComp;

    UPROPERTY(Transient)
    UCapsuleComponent* CapsuleComp;

    UPROPERTY(ReplicatedUsing = OnRep_IsJumping, VisibleAnywhere, BlueprintReadOnly, Category = "Jump")
    bool bIsJumping;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Jump")
    float PreviousZVelocity;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Jump")
    bool bWasOnGround;

    UFUNCTION()
    void OnRep_IsJumping();

    void HandleJumpLogic(float DeltaTime);
};