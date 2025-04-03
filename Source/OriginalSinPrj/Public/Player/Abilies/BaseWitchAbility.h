// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWitchAbility.generated.h"


UCLASS()
class ORIGINALSINPRJ_API ABaseWitchAbility : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseWitchAbility();

	void ExcuteAbility();
	void UndoAbility();

protected:
	virtual void BeginPlay() override;

};
