// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_SetBoolFlag.generated.h"

UCLASS()
class ORIGINALSINPRJ_API UAnimNotify_SetBoolFlag : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName BoolName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bNewValue = true;

	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
