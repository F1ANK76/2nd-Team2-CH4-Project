#pragma once

#include "CoreMinimal.h"
#include "../BaseWidget.h"
#include "IntroWidget.generated.h"

class UButton;

UCLASS()
class ORIGINALSINPRJ_API UIntroWidget : public UBaseWidget
{
	GENERATED_BODY()
	

public:
    virtual void InitWidget(UUIHandle* NewUIHandle) override;

private:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UButton> ToTitleButton;

public:
    virtual void NativeConstruct() override;
};
