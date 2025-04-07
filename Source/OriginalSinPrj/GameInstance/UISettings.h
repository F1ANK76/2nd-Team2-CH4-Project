// UISettings.h

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "UISettings.generated.h"

class UBaseWidget;

UCLASS(Config = UISetting, DefaultConfig, meta = (DisplayName = "UI Settings"))
class ORIGINALSINPRJ_API UUISettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Config, Category = "Widgets")
	TSubclassOf<UBaseWidget> TitleWidgetClass;
};
