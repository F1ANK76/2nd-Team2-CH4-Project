#include "IntroWidget.h"
#include "Components/Button.h"
//#include "System/UIHandle.h"


void UIntroWidget::InitWidget(UUISubsystem* uiSubsystem)
{
	Super::InitWidget(uiSubsystem);

	ToTitleButton->OnClicked.AddDynamic(this, &UBaseWidget::OnClickedMoveTitle);
}
