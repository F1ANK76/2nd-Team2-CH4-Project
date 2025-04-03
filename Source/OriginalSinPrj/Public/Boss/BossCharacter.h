// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossCharacter.generated.h"

UCLASS()
class ORIGINALSINPRJ_API ABossCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossCharacter();

	void UpdateFacingDirection(APawn* ClosestPlayer);

private:
	void SetFacingDirection(float Direction);
};