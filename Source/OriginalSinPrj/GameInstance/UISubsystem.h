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
	void CreateWidgets();

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBaseWidget> CurrentActiveWidget = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBaseWidget> TitleLevelWidget = nullptr;

	UWorld* World;

	const UUISettings* UISettings;

private:
	UFUNCTION()
	void OnPostLoadMap(UWorld* LoadedWorld);
};
