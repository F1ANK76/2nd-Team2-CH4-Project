// UISubsystem.h

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EnumSet.h"
#include "UISubsystem.generated.h"

class UTestBaseWidget;
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
	TObjectPtr<UTestBaseWidget> CurrentActiveWidget = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTestBaseWidget> TitleLevelWidget = nullptr;

	UWorld* World;

	const UUISettings* UISettings;

private:
	UFUNCTION()
	void OnPostLoadMap(UWorld* LoadedWorld);
};
