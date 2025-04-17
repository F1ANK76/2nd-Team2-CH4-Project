#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MonsterCharacter.generated.h"

class UKnockbackComponent;
class USphereComponent;
class UBuffComponent;

UCLASS()
class ORIGINALSINPRJ_API AMonsterCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AMonsterCharacter();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
    USphereComponent* AttackCollider;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knockback")
    float KnockbackIncreaseAmount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knockback")
    float BaseKnockbackStrength;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knockback")
    float KnockbackScaleFactor;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Knockback")
    UKnockbackComponent* KnockbackComp;

    UFUNCTION(BlueprintCallable, Category = "Platformer")
    void JumpToTargetPlatform(AActor* TargetActor);

    UFUNCTION()
    void OnAttackOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

    UFUNCTION(BlueprintCallable, Category = "Attack")
    void EnableAttackCollider();

    UFUNCTION(BlueprintCallable, Category = "Attack")
    void DisableAttackCollider();

    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
    bool bIsPlayerHit;

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool bIsAttack;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "State")
    bool bIsJumping;

    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "State")
    bool bIsMovingToBackLocation;

    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "State")
    bool bIsDead;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 ExpValue;

    UFUNCTION(BlueprintCallable, Server, Reliable)
    void ServerDie();

    UFUNCTION(NetMulticast, Reliable)
    void MulticastDie();

    UFUNCTION(BlueprintCallable)
    void DeactivateMonster();

    UFUNCTION(BlueprintCallable)
    void ActivateMonster();

    UFUNCTION(BlueprintCallable, Server, Reliable)
    void ServerGiveExpToPlayer(ACharacter* PlayerCharacter);

    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

    float Damage = 10.0f;

    float AddedKnockGauge = 2.0f;
    bool bIsActivedOverlap = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UBuffComponent> BuffComp = nullptr;

    UFUNCTION()
    void ApplyAttack(AActor* Target, float ApplyValue);

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Knockback")
    float KnockbackGauge = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knockback")
    float MaxKnockbackGauge = 200.0f;
};