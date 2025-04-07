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

            case ELevelType::MatchLevel:
                LevelName = "MatchLevel";
                CurrentActiveWidget = MatchLevelWidget;
                break;

            case ELevelType::LobbyLevel:
                LevelName = "LobbyLevel";
                CurrentActiveWidget = LobbyLevelWidget;
                break;

            case ELevelType::MultiLevel:
                LevelName = "MultiLevel";
                CurrentActiveWidget = MultiLevelWidget;
                break;

            case ELevelType::CooperationLevel:
                LevelName = "CooperationLevel";
                CurrentActiveWidget = CooperationLevelWidget;
                break;

            case ELevelType::SingleLevel:
                LevelName = "SingleLevel";
                CurrentActiveWidget = SingleLevelWidget;
                break;

            case ELevelType::TrainingLevel:
                LevelName = "TrainingLevel";
                CurrentActiveWidget = TrainingLevelWidget;
                break;

            case ELevelType::FarmingLevel:
                LevelName = "FarmingLevel";
                CurrentActiveWidget = FarmingLevelWidget;
                break;

            default:
                UE_LOG(LogTemp, Warning, TEXT("Invalid LevelType"));
                return;
        }

        // ���� �ε� ������ ���� ��� �ȵż� �ε� ���Ŀ� ������ ��쵵�� ���ε�
        FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UUISubsystem::OnPostLoadMap);

        // ���� ��ȯ�� ���� ���� ���� �ڵ� ���ŵ�
        UGameplayStatics::OpenLevel(World, FName(*LevelName));
    }
}

void UUISubsystem::OnPostLoadMap(UWorld* LoadedWorld)
{
    FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this); // ���� ��� �̺�Ʈ ���� ����

    if (CurrentActiveWidget)
    {
        CurrentActiveWidget->AddToViewport();
    }
}

void UUISubsystem::CreateWidgets()
{
    if (UISettings->TitleWidgetClass)
    {
        TitleLevelWidget = CreateWidget<UBaseWidget>(World, UISettings->TitleWidgetClass);
    }

    if (UISettings->MatchWidgetClass)
    {
        MatchLevelWidget = CreateWidget<UBaseWidget>(World, UISettings->MatchWidgetClass);
    }

    if (UISettings->LobbyWidgetClass)
    {
        LobbyLevelWidget = CreateWidget<UBaseWidget>(World, UISettings->LobbyWidgetClass);
    }

    if (UISettings->MultiWidgetClass)
    {
        MultiLevelWidget = CreateWidget<UBaseWidget>(World, UISettings->MultiWidgetClass);
    }

    if (UISettings->CooperationWidgetClass)
    {
        CooperationLevelWidget = CreateWidget<UBaseWidget>(World, UISettings->CooperationWidgetClass);
    }

    if (UISettings->SingleWidgetClass)
    {
        SingleLevelWidget = CreateWidget<UBaseWidget>(World, UISettings->SingleWidgetClass);
    }

    if (UISettings->TrainingWidgetClass)
    {
        TrainingLevelWidget = CreateWidget<UBaseWidget>(World, UISettings->TrainingWidgetClass);
    }

    if (UISettings->FarmingWidgetClass)
    {
        FarmingLevelWidget = CreateWidget<UBaseWidget>(World, UISettings->FarmingWidgetClass);
    }
}
