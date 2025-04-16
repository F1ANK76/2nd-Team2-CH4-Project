// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KillZone.generated.h"

class UBoxComponent;
class ALevelObjectManager;
class ABaseWitch;
class AMonsterCharacter;
UCLASS()
class ORIGINALSINPRJ_API AKillZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKillZone();

    UPROPERTY(VisibleAnywhere)
    class UBoxComponent* TriggerBox;

    UFUNCTION()
    void OnOverlapBegin(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

public:

	UFUNCTION(Server, Reliable)
	void Server_TriggerOverlapWitchEvent(ABaseWitch* WitchActor);

	UFUNCTION(Server, Reliable)
	void Server_TriggerOverlapMonsterEvent(AMonsterCharacter* Monster);

	UPROPERTY(EditAnywhere, Replicated, Category = "Platform")
	USceneComponent* Scene;

	UPROPERTY(EditAnywhere, Replicated, Category = "Platform")
	UBoxComponent* Box;
};
