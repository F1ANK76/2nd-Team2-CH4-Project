#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestPlatform.generated.h"

class UBoxComponent;

UCLASS()
class ORIGINALSINPRJ_API ATestPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	ATestPlatform();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(EditAnywhere, Replicated ,Category = "TestPlatform")
	USceneComponent* Scene;

	UPROPERTY(EditAnywhere, Replicated ,Category = "TestPlatform")
	UBoxComponent* Box;
	
	UPROPERTY(EditAnywhere, Replicated ,Category = "TestPlatform")
	UStaticMeshComponent* StaticMesh;
};
