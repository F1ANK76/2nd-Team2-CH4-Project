// UISubsystem.cpp

#include "UISubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "../Widget/BaseWidget.h"
#include "UISettings.h"

void UUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    World = GetGameInstance()->GetWorld();

    UISettings = GetDefault<UUISettings>();

    CreateWidgets();
}

void UUISubsystem::ShowLevel(ELevelType LevelType)
{
    if (World)
    {
        FString LevelName;

        switch (LevelType)
        {
            case ELevelType::TitleLevel:
                LevelName = "TitleLevel";
                CurrentActiveWidget = TitleLevelWidget;
                break;

            default:
                UE_LOG(LogTemp, Warning, TEXT("Invalid LevelType"));
                return;
        }

        // 레벨 로딩 이전에 위젯 등록 안돼서 로딩 이후에 위젯을 띄우도록 바인딩
        FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UUISubsystem::OnPostLoadMap);

        // 레벨 전환시 기존 위젯 전부 자동 제거됨
        UGameplayStatics::OpenLevel(World, FName(*LevelName));
    }
}

void UUISubsystem::OnPostLoadMap(UWorld* LoadedWorld)
{
    FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this); // 이전 등록 이벤트 전부 제거

    if (CurrentActiveWidget)
    {
        CurrentActiveWidget->AddToViewport();
    }
}

void UUISubsystem::CreateWidgets()
{
    if (UISettings)
    {
        if (UISettings->TitleWidgetClass)
        {
            TitleLevelWidget = CreateWidget<UBaseWidget>(World, UISettings->TitleWidgetClass);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Invalid TitleWidgetClass"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid UISettings"));
    }
}
