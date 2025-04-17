#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KnockbackComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ORIGINALSINPRJ_API UKnockbackComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UKnockbackComponent();

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Knockback")
    float KnockbackGauge;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knockback")
    float MaxKnockbackGauge;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knockback")
    float MinZKnockback;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knockback")
    float ZKnockbackStrengthMultiplier;

    UFUNCTION(BlueprintCallable, Category = "Knockback", Server, Reliable, WithValidation)
    void AddKnockbackGauge(float Amount);

    bool AddKnockbackGauge_Validate(float Amount);
    void AddKnockbackGauge_Implementation(float Amount);

    UFUNCTION(BlueprintCallable, Category = "Knockback", Server, Reliable, WithValidation)
    void ApplyKnockback(FVector Direction, float BaseStrength);

    bool ApplyKnockback_Validate(FVector Direction, float BaseStrength);
    void ApplyKnockback_Implementation(FVector Direction, float BaseStrength);

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};