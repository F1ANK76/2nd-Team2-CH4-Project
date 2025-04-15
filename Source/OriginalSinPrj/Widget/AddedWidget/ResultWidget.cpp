// Fill out your copyright notice in the Description page of Project Settings.


#include "ResultWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UResultWidget::InitWidget(UUISubsystem* uiSubsystem)
{
	OpenAnimation = OpenAnim;

	Super::InitWidget(uiSubsystem);

	MoveNextButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveNext);
	MoveTitleButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveTitle);
}

void UResultWidget::UpdateResult(bool bIsClear)
{
	MoveNextButton->OnClicked.Clear();

	if (bIsClear) // �¸���
	{

	}
	else // �й��
	{

	}
}

void UResultWidget::OnClickedRetry()
{
	//UISubsystem �Ǵ� UIHandler�� ���� ���� Ȯ��
	// ���� �ٽ� �����ϰ� ���ּ��� ��û
	
	//checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	//UIHandle->ClickedRetry();
}

