#pragma once

#include "CoreMinimal.h"
#include "../BaseWidget.h"
#include "IntroWidget.generated.h"

class UButton;

UCLASS()
class ORIGINALSINPRJ_API UIntroWidget : public UBaseWidget
{
	GENERATED_BODY()
	

protected:
    virtual void NativeConstruct() override;

public:


private:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UButton> ToTitleButton;


};
