// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_JumpToSection.generated.h"

UCLASS()
class ORIGINALSINPRJ_API UAnimNotify_JumpToSection : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TargetSection;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	virtual FString GetNotifyName_Implementation() const override;
};
