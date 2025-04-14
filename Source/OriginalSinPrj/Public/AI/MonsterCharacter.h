//#pragma once
//
//#include "CoreMinimal.h"
//#include "GameFramework/Character.h"
//#include "MonsterCharacter.generated.h"
//
//class UKnockbackComponent;
//class ATestCharacter;
//class USphereComponent;
//
//UCLASS()
//class ORIGINALSINPRJ_API AMonsterCharacter : public ACharacter
//{
//    GENERATED_BODY()
//
//public:
//    AMonsterCharacter();
//
//    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
//    USphereComponent* AttackCollider;
//
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knockback")
//    float KnockbackIncreaseAmount;
//
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knockback")
//    float BaseKnockbackStrength;
//
//    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Knockback")
//    UKnockbackComponent* KnockbackComp;
//
//    UFUNCTION(BlueprintCallable, Category = "Platformer")
//    void JumpToTargetPlatform(AActor* TargetActor);
//
//    UFUNCTION()
//    void OnAttackOverlap(
//        UPrimitiveComponent* OverlappedComponent,
//        AActor* OtherActor,
//        UPrimitiveComponent* OtherComp,
//        int32 OtherBodyIndex,
//        bool bFromSweep,
//        const FHitResult& SweepResult);
//
//    UFUNCTION(BlueprintCallable, Category = "Attack")
//    void EnableAttackCollider();
//
//    UFUNCTION(BlueprintCallable, Category = "Attack")
//    void DisableAttackCollider();
//
//    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
//    bool bIsPlayerHit;
//    
//    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Animation")
//    bool bIsAttack;
//
//    // 리플리케이션
//    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
//
//    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "State")
//    bool bIsJumping;
//
//    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "State")
//    bool bIsMovingToBackLocation;
//
//    // Dead 상태 추가
//    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "State")
//    bool bIsDead;
//
//    // 경험치 값 추가
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
//    int32 ExpValue;
//
//    // 서버에서 몬스터 사망 처리
//    UFUNCTION(BlueprintCallable, Server, Reliable)
//    void ServerDie();
//
//    // 클라이언트에서 사망 상태 동기화
//    UFUNCTION(NetMulticast, Reliable)
//    void MulticastDie();
//
//    // 풀링을 위해 비활성화
//    UFUNCTION(BlueprintCallable)
//    void DeactivateMonster();
//
//    // 풀링을 위해 활성화
//    UFUNCTION(BlueprintCallable)
//    void ActivateMonster();
//
//    // 경험치 획득 처리
//    UFUNCTION(BlueprintCallable, Server, Reliable)
//    void ServerGiveExpToPlayer(ACharacter* PlayerCharacter);
//};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MonsterCharacter.generated.h"

class UKnockbackComponent;
class USphereComponent;

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

    // 리플리케이션
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "State")
    bool bIsJumping;

    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "State")
    bool bIsMovingToBackLocation;

    // Dead 상태 추가
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "State")
    bool bIsDead;

    // 경험치 값 추가
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 ExpValue;

    // 서버에서 몬스터 사망 처리
    UFUNCTION(BlueprintCallable, Server, Reliable)
    void ServerDie();

    // 클라이언트에서 사망 상태 동기화
    UFUNCTION(NetMulticast, Reliable)
    void MulticastDie();

    // 풀링을 위해 비활성화
    UFUNCTION(BlueprintCallable)
    void DeactivateMonster();

    // 풀링을 위해 활성화
    UFUNCTION(BlueprintCallable)
    void ActivateMonster();

    // 경험치 획득 처리
    UFUNCTION(BlueprintCallable, Server, Reliable)
    void ServerGiveExpToPlayer(ACharacter* PlayerCharacter);
};