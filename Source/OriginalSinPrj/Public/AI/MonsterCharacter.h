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
//    // ���ø����̼�
//    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
//
//    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "State")
//    bool bIsJumping;
//
//    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "State")
//    bool bIsMovingToBackLocation;
//
//    // Dead ���� �߰�
//    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "State")
//    bool bIsDead;
//
//    // ����ġ �� �߰�
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
//    int32 ExpValue;
//
//    // �������� ���� ��� ó��
//    UFUNCTION(BlueprintCallable, Server, Reliable)
//    void ServerDie();
//
//    // Ŭ���̾�Ʈ���� ��� ���� ����ȭ
//    UFUNCTION(NetMulticast, Reliable)
//    void MulticastDie();
//
//    // Ǯ���� ���� ��Ȱ��ȭ
//    UFUNCTION(BlueprintCallable)
//    void DeactivateMonster();
//
//    // Ǯ���� ���� Ȱ��ȭ
//    UFUNCTION(BlueprintCallable)
//    void ActivateMonster();
//
//    // ����ġ ȹ�� ó��
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

    // ���ø����̼�
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "State")
    bool bIsJumping;

    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "State")
    bool bIsMovingToBackLocation;

    // Dead ���� �߰�
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "State")
    bool bIsDead;

    // ����ġ �� �߰�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 ExpValue;

    // �������� ���� ��� ó��
    UFUNCTION(BlueprintCallable, Server, Reliable)
    void ServerDie();

    // Ŭ���̾�Ʈ���� ��� ���� ����ȭ
    UFUNCTION(NetMulticast, Reliable)
    void MulticastDie();

    // Ǯ���� ���� ��Ȱ��ȭ
    UFUNCTION(BlueprintCallable)
    void DeactivateMonster();

    // Ǯ���� ���� Ȱ��ȭ
    UFUNCTION(BlueprintCallable)
    void ActivateMonster();

    // ����ġ ȹ�� ó��
    UFUNCTION(BlueprintCallable, Server, Reliable)
    void ServerGiveExpToPlayer(ACharacter* PlayerCharacter);
};