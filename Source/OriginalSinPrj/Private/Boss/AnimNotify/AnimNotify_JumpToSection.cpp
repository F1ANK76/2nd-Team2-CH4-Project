// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/AnimNotify/AnimNotify_JumpToSection.h"

void UAnimNotify_JumpToSection::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!IsValid(MeshComp) || TargetSection.IsNone()) return;

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (!IsValid(AnimInstance)) return;

	UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage();
	if (!IsValid(CurrentMontage)) return;
	
	AnimInstance->Montage_JumpToSection(TargetSection, CurrentMontage);
}

FString UAnimNotify_JumpToSection::GetNotifyName_Implementation() const
{
	return FString::Printf(TEXT("JumpToSection : %s"), *TargetSection.ToString());
}
