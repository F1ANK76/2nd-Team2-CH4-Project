#include "BaseWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/WidgetAnimation.h"
#include "../GameInstance/UISubsystem.h"

void UBaseWidget::NativeConstruct()
{
    Super::NativeConstruct();

	UIHandle = GetGameInstance()->GetSubsystem<UUISubsystem>();
	/*
	UIHandle = GetGameInstance()->GetSubsystem<UUISubsystem>();
	StartAddDelegate.BindDynamic(this, &ThisClass::StartAddAnim);
	EndAddDelegate.BindDynamic(this, &ThisClass::EndAddAnim);
	StartRemoveDelegate.BindDynamic(this, &ThisClass::StartRemoveAnim);
	EndRemoveDelegate.BindDynamic(this, &ThisClass::EndRemoveAnim);

	BindToAnimationStarted(OpenAnimation, StartAddDelegate);
	BindToAnimationFinished(OpenAnimation, EndAddDelegate);
	BindToAnimationStarted(CloseAnimation, StartRemoveDelegate);
	BindToAnimationFinished(CloseAnimation, EndRemoveDelegate);
	*/
}


//////////////////////////


void UBaseWidget::InitWidget(UUISubsystem* uiHandle)
{
	/*
	UIHandle = uiHandle;
	StartAddDelegate.BindDynamic(this, &ThisClass::StartAddAnim);
	EndAddDelegate.BindDynamic(this, &ThisClass::EndAddAnim);
	StartRemoveDelegate.BindDynamic(this, &ThisClass::StartRemoveAnim);
	EndRemoveDelegate.BindDynamic(this, &ThisClass::EndRemoveAnim);

	BindToAnimationStarted(OpenAnimation, StartAddDelegate);
	BindToAnimationFinished(OpenAnimation, EndAddDelegate);
	BindToAnimationStarted(CloseAnimation, StartRemoveDelegate);
	BindToAnimationFinished(CloseAnimation, EndRemoveDelegate);
	*/
}

void UBaseWidget::Action()
{

}

void UBaseWidget::StartAddAnim()
{
	bIsPlaying = true;
}

void UBaseWidget::EndAddAnim()
{
	bIsPlaying = false;
	//checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
}

void UBaseWidget::Update()
{

}

void UBaseWidget::PlayUIOpenAnimation()
{
	if (bIsPlaying)
	{
		return;
	}
	PlayAnimation(OpenAnimation);
}

void UBaseWidget::PlayUICloseAnimation()
{
	if (bIsPlaying)
	{
		return;
	}
	PlayAnimation(CloseAnimation);
}

void UBaseWidget::PlayUIOpenAnimation(UWidgetAnimation* UIOpenAnimation)
{
	if (bIsPlaying)
	{
		return;
	}
	PlayAnimation(UIOpenAnimation);
}

void UBaseWidget::PlayUICloseAnimation(UWidgetAnimation* UIcloseAnimation)
{
	if (bIsPlaying)
	{
		return;
	}
	PlayAnimation(UIcloseAnimation);
}


void UBaseWidget::PlayAddAnim()
{
	if (bIsPlaying)
	{
		return;
	}
	PlayAnimation(OpenAnimation);
}


/*
void UBaseWidget::PlayRemoveAnim(bool bIsNext, ESceneType SceneType)
{
	if (bIsPlaying)
	{
		return;
	}
	PlayAnimation(CloseAnimation);
}
*/

void UBaseWidget::PlayRemoveAnim()
{
	if (bIsPlaying)
	{
		return;
	}
	PlayAnimation(CloseAnimation);
}

void UBaseWidget::StartRemoveAnim()
{
	bIsPlaying = true;
}

void UBaseWidget::EndRemoveAnim()
{
	bIsPlaying = false;
	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
}

void UBaseWidget::OnClickedMoveNext()
{
	//UISubsystemȤ�� UIhandler üũ
	// UISubsystemȤ�� UIhandler�� Ŭ���ߴٰ� �˸��� ���� �ߵ� ������Ʈ.
	// �����ȯ �Լ� ���� ��û
	// 
	
	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	//UIHandle->RequestPlayUISound(EUISoundType::Click);
	//UIHandle->ClickedMoveToNext();
}

void UBaseWidget::OnClickedMoveSingleMode()
{
	//fade out
	//�� ��ȯ? Ȥ�� ���� ��ȯ? UIsubsystem Ȥ�� UIhandler�� ��û
	//fade in

	UIHandle->ShowLevel(ELevelType::SingleLevel);
}


void UBaseWidget::OnClickedMoveMultiMode()
{
	//fade out
	//�� ��ȯ? Ȥ�� ���� ��ȯ? UIsubsystem Ȥ�� UIhandler�� ��û
	//fade in
	UIHandle->ShowLevel(ELevelType::MultiLevel);
}


void UBaseWidget::OnClickedMoveTrainingMode()
{
	//fade out
	//�� ��ȯ? Ȥ�� ���� ��ȯ? UIsubsystem Ȥ�� UIhandler�� ��û
	//fade in
	UIHandle->ShowLevel(ELevelType::TrainingLevel);
}

void UBaseWidget::OnClickedMoveTitle()
{
	//UISubsystemȤ�� UIhandler üũ
	// UISubsystemȤ�� UIhandler�� Ŭ���ߴٰ� �˸��� ���� �ߵ� ������Ʈ.
	// Ÿ��Ʋ ���� ��ȯ �Լ� ���� ��û
 
	//checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	//UIHandle->RequestPlayUISound(EUISoundType::Click);
	//UIHandle->ClickedMoveToTitle();

	UE_LOG(LogTemp, Warning, TEXT("ShowLevel called - this: %p"), this);

	UIHandle->ShowLevel(ELevelType::TitleLevel);

}

void UBaseWidget::OnClickedQuitGame()
{
	//UISubsystemȤ�� UIhandler üũ
	// UISubsystemȤ�� UIhandler�� Ŭ���ߴٰ� �˸��� ���� �ߵ� ������Ʈ.
	// ���� ���� �Լ� ���� ��û


	//checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	//UIHandle->RequestPlayUISound(EUISoundType::Click);
	UIHandle->CloseGame();

}


void UBaseWidget::OnClickedOpenWidget(const EWidgetType WidgetType)
{
	//checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	//UIHandle->AddToViewportByCoverType(WidgetType);
}

void UBaseWidget::OnClickedCloseWidget(const EWidgetType WidgetType)
{
	//checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	//UIHandle->RequestRemoveCoverFromViewport(WidgetType);
}




void UBaseWidget::OnClick_MoveLevel()
{
	//if (UISubsystem�� �����ϸ�)
	{
		/*UISubSystem�� ��û*/
		//UISub->RequestLevelChange("LevelName");
	}
}

void UBaseWidget::OnClickedOptionButton()
{
	UIHandle->ShowWidget(EAddWidgetType::OptionWidget);

}