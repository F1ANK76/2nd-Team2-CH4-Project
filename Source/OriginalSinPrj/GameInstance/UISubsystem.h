// UISubsystem.h

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EnumSet.h"
#include "UISubsystem.generated.h"

class UBaseWidget;
class UUISettings;

UCLASS()
class ORIGINALSINPRJ_API UUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void ShowLevel(ELevelType LevelType);
	void ShowWidget(EAddWidgetType WidgetType);
	void CreateWidgets();

	UFUNCTION(BlueprintCallable)
	void CloseGame();

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBaseWidget> CurrentActiveWidget = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBaseWidget> IntroLevelWidget = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBaseWidget> TitleLevelWidget = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBaseWidget> MatchLevelWidget = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBaseWidget> LobbyLevelWidget = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBaseWidget> MultiLevelWidget = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBaseWidget> CooperationLevelWidget = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBaseWidget> SingleLevelWidget = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBaseWidget> TrainingLevelWidget = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBaseWidget> FarmingLevelWidget = nullptr;

	// On/Off Widget

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBaseWidget> OptionWidget = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBaseWidget> BuffSelectWidget = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBaseWidget> CharacterSelectWidget = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBaseWidget> GameSettingWidget = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBaseWidget> MapSelectWidget = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBaseWidget> PlayerStateWidget = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBaseWidget> ResultWidget = nullptr;

	//UWorld* World;

	const UUISettings* UISettings;

private:
	UFUNCTION()
	void OnPostLoadMap(UWorld* LoadedWorld);
};
