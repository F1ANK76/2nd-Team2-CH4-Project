// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWitchAbility.generated.h"

class ABaseWitch;

UCLASS()
class ORIGINALSINPRJ_API ABaseWitchAbility : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseWitchAbility();

	virtual void InitAbility(ABaseWitch* NewParent);
	virtual void ExcuteAbility(const FVector2D& DirectionVector);
	virtual void UndoAbility();

protected:
	virtual void BeginPlay() override;


protected:
	TObjectPtr<ABaseWitch> ParentWitch = nullptr;
};
