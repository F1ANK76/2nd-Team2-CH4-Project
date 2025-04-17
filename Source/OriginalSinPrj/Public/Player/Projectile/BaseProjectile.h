// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/WitchTypes.h"
#include "OriginalSinPrj/GameInstance/EnumSet.h"
#include "BaseProjectile.generated.h"

class UNiagaraComponent;
class UBoxComponent;
class ABaseWitch;
class UProjectileMovementComponent;
class UAudioComponent;
struct FAbilityDataBuffer;
struct FProjectileDataBuffer;

UCLASS()
class ORIGINALSINPRJ_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseProjectile();

	virtual void InitProjectile(ABaseWitch* Parent) {};
	virtual void ActiveProjectile(const FProjectileDataBuffer& Buffer);
	virtual void ActiveProjectile(const FAbilityDataBuffer& Buffer, float DelayTime) {};
	virtual void DeactiveProjectile();

	EProjectileType GetProjectileType() const;
	bool GetIsActevated() const;

protected:
	UFUNCTION(NetMulticast, Unreliable)
	void OnActivedProjectile();

	UFUNCTION(NetMulticast, Unreliable)
	void OnDeactivedProjectile();

	//UFUNCTION(NetMulticast, Unreliable)
	//void ResponseActivateMove(const FVector& Direction, const float& SpeedValue);

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnEndedOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void BeginPlay() override;
	virtual void ApplyMove();
	virtual void ApplyAttack();
	virtual void ApplyVisible();

	void ActiveMoveDelayTimer(float TimeValue);
	void ActiveAttackDelayTimer(float TimeValue);
	void ActiveVisibleDelayTimer(float TimeValue);
	void ActiveDeactiveDelayTimer(float TimeValue);

	void ResetProjectile();

	void RequestPlayStratSound();
	void RequestPlayAttackSound();
	void RequestPlayEndSound();
	

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UNiagaraComponent> EffectComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> CollisionComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> SceneComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UAudioComponent> AudioComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> MoveComp = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	EProjectileType ProjectileType = EProjectileType::Normal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool bIsNotDelayDeactiveFromOverlap = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool bIsAgainAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	ECharacterSoundType StartSoundType = ECharacterSoundType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	ECharacterSoundType AttackSoundType = ECharacterSoundType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	ECharacterSoundType EndSoundType = ECharacterSoundType::None;

protected:
	UPROPERTY()
	TObjectPtr<ABaseWitch> ParentWitch = nullptr;

	bool bActiveOverlapEvent = false;
	bool bCompleteDeactive = false;
	bool bIsActivated = false;

	FTimerHandle DeactiveTimer;
	FTimerHandle MoveDelayTimer;
	FTimerHandle AttackDelayTimer;
	FTimerHandle VisibleDelayTimer;

	FVector MoveDirection = FVector::ZeroVector;
	float MoveSpeed = 0.0f;
	float AttackDamage = 0.0f;
};
