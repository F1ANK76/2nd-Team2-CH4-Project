// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossPoolableActorInterface.h"
#include "GameFramework/Actor.h"
#include "BossPlatform.generated.h"

UCLASS()
class ORIGINALSINPRJ_API ABossPlatform : public AActor, public IBossPoolableActorInterface
{
	GENERATED_BODY()
	
public:	
	ABossPlatform();

protected:
	virtual void BeginPlay() override;
	virtual void OnPooledObjectSpawn_Implementation() override;
	virtual void OnPooledObjectReset_Implementation() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneRoot = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Property")
	float DisappearDelay = 2.0f;

	UFUNCTION()
	void OnPlatformHit(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetActive(bool bIsActive);

	void StartDisappearTimer();
	void DisappearPlatform();

private:
	bool bIsActivate = false;
	bool bHasBeenTriggered = false;
	FTimerHandle DisappearTimerHandle;
};
