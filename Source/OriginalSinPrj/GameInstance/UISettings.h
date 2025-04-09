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
	TSubclassOf<UBaseWidget> IntroWidgetClass;

	UPROPERTY(EditAnywhere, Config, Category = "Widgets")
	TSubclassOf<UBaseWidget> TitleWidgetClass;

	UPROPERTY(EditAnywhere, Config, Category = "Widgets")
	TSubclassOf<UBaseWidget> MatchWidgetClass;

	UPROPERTY(EditAnywhere, Config, Category = "Widgets")
	TSubclassOf<UBaseWidget> LobbyWidgetClass;

	UPROPERTY(EditAnywhere, Config, Category = "Widgets")
	TSubclassOf<UBaseWidget> MultiWidgetClass;

	UPROPERTY(EditAnywhere, Config, Category = "Widgets")
	TSubclassOf<UBaseWidget> CooperationWidgetClass;

	UPROPERTY(EditAnywhere, Config, Category = "Widgets")
	TSubclassOf<UBaseWidget> SingleWidgetClass;

	UPROPERTY(EditAnywhere, Config, Category = "Widgets")
	TSubclassOf<UBaseWidget> TrainingWidgetClass;

	UPROPERTY(EditAnywhere, Config, Category = "Widgets")
	TSubclassOf<UBaseWidget> FarmingWidgetClass;

	// On/Off Widget

	UPROPERTY(EditAnywhere, Config, Category = "On/Off Widgets")
	TSubclassOf<UBaseWidget> OptionWidgetClass;

	UPROPERTY(EditAnywhere, Config, Category = "On/Off Widgets")
	TSubclassOf<UBaseWidget> BuffSelectWidgetClass;

	UPROPERTY(EditAnywhere, Config, Category = "On/Off Widgets")
	TSubclassOf<UBaseWidget> CharacterSelectWidgetClass;

	UPROPERTY(EditAnywhere, Config, Category = "On/Off Widgets")
	TSubclassOf<UBaseWidget> GameSettingWidgetClass;

	UPROPERTY(EditAnywhere, Config, Category = "On/Off Widgets")
	TSubclassOf<UBaseWidget> MapSelectWidgetClass;

	UPROPERTY(EditAnywhere, Config, Category = "On/Off Widgets")
	TSubclassOf<UBaseWidget> PlayerStateWidgetClass;

	UPROPERTY(EditAnywhere, Config, Category = "On/Off Widgets")
	TSubclassOf<UBaseWidget> ResultWidgetClass;
};
