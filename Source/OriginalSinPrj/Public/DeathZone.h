#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DeathZone.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnMyOverlapEvent);

class UBoxComponent;
class ALevelObjectManager;
class ABaseWitch;

UCLASS()
class ORIGINALSINPRJ_API ADeathZone : public AActor
{
	GENERATED_BODY()
	
public:	
	ADeathZone();

	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

public:	
	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void TriggerOverlapPlatformEvent();

	FOnMyOverlapEvent OnMyOverlapEvent;

	UPROPERTY(EditAnywhere, Replicated, Category = "Platform")
	USceneComponent* Scene;

	UPROPERTY(EditAnywhere, Replicated, Category = "Platform")
	UBoxComponent* Box;

	UPROPERTY(EditAnywhere, Replicated, Category = "Platform")
	UStaticMeshComponent* StaticMesh;

private:
	float MoveSpeed;

};
