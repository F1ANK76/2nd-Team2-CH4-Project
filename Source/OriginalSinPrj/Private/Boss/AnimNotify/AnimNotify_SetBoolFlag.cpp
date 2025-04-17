// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/AnimNotify/AnimNotify_SetBoolFlag.h"

FString UAnimNotify_SetBoolFlag::GetNotifyName_Implementation() const
{
	return FString::Printf(TEXT("Set %s = %s"), *BoolName.ToString(), bNewValue ? TEXT("true") : TEXT("false"));
}

void UAnimNotify_SetBoolFlag::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!IsValid(MeshComp)) return;

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (!IsValid(AnimInstance) || BoolName.IsNone()) return;

	// 변수 이름을 기반으로 bool 변수 찾기
	for (TFieldIterator<FBoolProperty> PropIt(AnimInstance->GetClass()); PropIt; ++PropIt)
	{
		FBoolProperty* BoolProp = *PropIt;
		if (BoolProp && BoolProp->GetFName() == BoolName)
		{
			BoolProp->SetPropertyValue_InContainer(AnimInstance, bNewValue);
			return;
		}
	}
}
