#include "IntroWidget.h"
#include "Components/Button.h"
//#include "System/UIHandle.h"


void UIntroWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ToTitleButton->OnClicked.RemoveDynamic(this, &UBaseWidget::OnClickedMoveTitle);
	ToTitleButton->OnClicked.AddDynamic(this, &UBaseWidget::OnClickedMoveTitle);

}

