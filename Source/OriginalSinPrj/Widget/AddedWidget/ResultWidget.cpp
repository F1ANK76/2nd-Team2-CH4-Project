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
    PlayUIEffectSound(EUISfxSoundType::Click);
	//UISubsystem �Ǵ� UIHandler�� ���� ���� Ȯ��
	// ���� �ٽ� �����ϰ� ���ּ��� ��û
	
	//checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	//UIHandle->ClickedRetry();
}
void UResultWidget::InitResultWidgetData(
    float InPlayer1Damage,
    float InPlayer2Damage,
    int32 InPlayer1Death,
    int32 InPlayer2Death,
    int32 InPlayer1AttackCount,
    int32 InPlayer2AttackCount,
    float InStage1Timer,
    float InStage2Timer,
    float InStage3Timer,
    bool bStage1Started,
    bool bStage2Started,
    bool bStage3Started)
{
    if (Player1ReceivedDamage)
        Player1ReceivedDamage->SetText(FText::AsNumber(FMath::RoundToInt(InPlayer1Damage)));

    if (Player2ReceivedDamage)
        Player2ReceivedDamage->SetText(FText::AsNumber(FMath::RoundToInt(InPlayer2Damage)));

    if (Player1Death)
        Player1Death->SetText(FText::AsNumber(InPlayer1Death));

    if (Player2Death)
        Player2Death->SetText(FText::AsNumber(InPlayer2Death));

    if (Player1AttackCount)
        Player1AttackCount->SetText(FText::AsNumber(InPlayer1AttackCount));

    if (Player2AttackCount)
        Player2AttackCount->SetText(FText::AsNumber(InPlayer2AttackCount));

    if (Stage1Timer)
        Stage1Timer->SetText(bStage1Started ? FText::AsNumber(FMath::RoundToInt(InStage1Timer)) : FText::FromString(TEXT("-")));

    if (Stage2Timer)
        Stage2Timer->SetText(bStage2Started ? FText::AsNumber(FMath::RoundToInt(InStage2Timer)) : FText::FromString(TEXT("-")));

    if (Stage3Timer)
        Stage3Timer->SetText(bStage3Started ? FText::AsNumber(FMath::RoundToInt(InStage3Timer)) : FText::FromString(TEXT("-")));
}