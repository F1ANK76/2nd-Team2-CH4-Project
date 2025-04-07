// Fill out your copyright notice in the Description page of Project Settings.


#include "ResultWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UResultWidget::InitWidget(UUIHandle* NewUIHandle)
{
	OpenAnimation = OpenAnim;

	Super::InitWidget(NewUIHandle);

	MoveNextButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveNext);
	MoveTitleButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveTitle);
}



void UResultWidget::UpdateResult(bool bIsClear)
{
	MoveNextButton->OnClicked.Clear();

	if (bIsClear) // 승리시
	{

	}
	else // 패배시
	{

	}
}

void UResultWidget::OnClickedRetry()
{
	//UISubsystem 또는 UIHandler의 존재 여부 확인
	// 게임 다시 시작하게 해주세요 요청
	
	//checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	//UIHandle->ClickedRetry();
}

