// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

class UNiagaraComponent;
class UBoxComponent;
class ABaseWitch;
struct FAbilityDataBuffer;

UCLASS()
class ORIGINALSINPRJ_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseProjectile();

	virtual void InitProjectile(ABaseWitch* Parent);
	virtual void ActiveProjectile(const FAbilityDataBuffer& Buffer, float DelayTime);
	virtual void DeactiveProjectile();

protected:
	UFUNCTION(NetMulticast, Unreliable)
	void OnActivedProjectile();

	UFUNCTION(NetMulticast, Unreliable)
	void OnDeactivedProjectile();

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UNiagaraComponent> EffectComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> CollisionComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> SceneComp = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float DefaultDamage = 0;

protected:
	UPROPERTY()
	TObjectPtr<ABaseWitch> ParentWitch = nullptr;

	FTimerHandle DelayTimer;

	bool bActiveOverlapEvent = false;
};
