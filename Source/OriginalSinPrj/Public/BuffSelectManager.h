// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuffSelectManager.generated.h"

UCLASS()
class ORIGINALSINPRJ_API ABuffSelectManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuffSelectManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void RequestBuffSelect(int32 BuffIndex);

	UFUNCTION(Server, Reliable)
	void Server_RequestBuffSelect(int32 BuffIndex);


};
