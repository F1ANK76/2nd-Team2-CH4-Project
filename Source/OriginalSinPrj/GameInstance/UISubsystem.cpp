// UISubsystem.cpp

#include "UISubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "../Widget/BaseWidget.h"
#include "UISettings.h"
#include "AudioSubsystem.h"

void UUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    //World = GetGameInstance()->GetWorld();

    UISettings = GetDefault<UUISettings>();

    CreateWidgets();
}

void UUISubsystem::ShowLevel(ELevelType LevelType)
{
    if (GetWorld())
    {
        FString LevelName;

        switch (LevelType)
        {
            case ELevelType::IntroLevel:
                LevelName = "IntroLevel";
                CurrentActiveWidget = IntroLevelWidget;
                break;

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

        // 레벨 로딩 이전에 위젯 등록 안돼서 로딩 이후에 위젯을 띄우도록 바인딩
        FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UUISubsystem::OnPostLoadMap);

        // 레벨 전환시 기존 위젯 전부 자동 제거됨
        UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid World"));
    }
}

void UUISubsystem::OnPostLoadMap(UWorld* LoadedWorld)
{
    FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this); // 이전 등록 이벤트 전부 제거

    if (CurrentActiveWidget)
    {
        CurrentActiveWidget->AddToViewport();
        
        if (UAudioSubsystem* AudioSubsystem = GetGameInstance()->GetSubsystem<UAudioSubsystem>())
        {
            // BGM 재생 호출 시점 변경은 필요할 수도 있음, 레벨 전환시 기존에 재생되던 BGM은 전부 날아감
            if (CurrentActiveWidget == IntroLevelWidget)
            {
                AudioSubsystem->PlayBGM(ELevelSoundType::TitleSound);
            }

            // 필요 레벨별 재생..
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid CurrentActiveWidget"));
    }
}

void UUISubsystem::ShowWidget(EAddWidgetType WidgetType)
{
    UBaseWidget* TargetWidget = nullptr;

    switch (WidgetType)
    {
        case EAddWidgetType::OptionWidget:     
            TargetWidget = OptionWidget; 
            break;

        case EAddWidgetType::BuffSelectWidget:
            TargetWidget = BuffSelectWidget;
            break;

        case EAddWidgetType::CharacterSelectWidget:
            TargetWidget = CharacterSelectWidget;
            break;

        case EAddWidgetType::GameSettingWidget:
            TargetWidget = GameSettingWidget;
            break;

        case EAddWidgetType::MapSelectWidget:
            TargetWidget = MapSelectWidget;
            break;

        case EAddWidgetType::PlayerStateWidget:
            TargetWidget = PlayerStateWidget;
            break;

        case EAddWidgetType::ResultWidget:
            TargetWidget = ResultWidget;
            break;

        default:
            UE_LOG(LogTemp, Warning, TEXT("Invalid WidgetType"));
            return;
    }

    if (TargetWidget)
    {
        if (!TargetWidget->IsInViewport())
        {
            TargetWidget->AddToViewport();
        }
        else
        {
            TargetWidget->RemoveFromParent();
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid TargetWidget"));
    }
}


void UUISubsystem::CreateWidgets()
{
    if (UISettings->IntroWidgetClass)
    {
        IntroLevelWidget = CreateWidget<UBaseWidget>(GetWorld(), UISettings->IntroWidgetClass);
    }

    if (UISettings->TitleWidgetClass)
    {
        TitleLevelWidget = CreateWidget<UBaseWidget>(GetWorld(), UISettings->TitleWidgetClass);
    }

    if (UISettings->MatchWidgetClass)
    {
        MatchLevelWidget = CreateWidget<UBaseWidget>(GetWorld(), UISettings->MatchWidgetClass);
    }

    if (UISettings->LobbyWidgetClass)
    {
        LobbyLevelWidget = CreateWidget<UBaseWidget>(GetWorld(), UISettings->LobbyWidgetClass);
    }

    if (UISettings->MultiWidgetClass)
    {
        MultiLevelWidget = CreateWidget<UBaseWidget>(GetWorld(), UISettings->MultiWidgetClass);
    }

    if (UISettings->CooperationWidgetClass)
    {
        CooperationLevelWidget = CreateWidget<UBaseWidget>(GetWorld(), UISettings->CooperationWidgetClass);
    }

    if (UISettings->SingleWidgetClass)
    {
        SingleLevelWidget = CreateWidget<UBaseWidget>(GetWorld(), UISettings->SingleWidgetClass);
    }

    if (UISettings->TrainingWidgetClass)
    {
        TrainingLevelWidget = CreateWidget<UBaseWidget>(GetWorld(), UISettings->TrainingWidgetClass);
    }

    if (UISettings->FarmingWidgetClass)
    {
        FarmingLevelWidget = CreateWidget<UBaseWidget>(GetWorld(), UISettings->FarmingWidgetClass);
    }

    // On/Off Widget

    if (UISettings->OptionWidgetClass)
    {
        OptionWidget = CreateWidget<UBaseWidget>(GetWorld(), UISettings->OptionWidgetClass);
    }

    if (UISettings->BuffSelectWidgetClass)
    {
        BuffSelectWidget = CreateWidget<UBaseWidget>(GetWorld(), UISettings->BuffSelectWidgetClass);
    }

    if (UISettings->CharacterSelectWidgetClass)
    {
        CharacterSelectWidget = CreateWidget<UBaseWidget>(GetWorld(), UISettings->CharacterSelectWidgetClass);
    }

    if (UISettings->GameSettingWidgetClass)
    {
        GameSettingWidget = CreateWidget<UBaseWidget>(GetWorld(), UISettings->GameSettingWidgetClass);
    }

    if (UISettings->MapSelectWidgetClass)
    {
        MapSelectWidget = CreateWidget<UBaseWidget>(GetWorld(), UISettings->MapSelectWidgetClass);
    }

    if (UISettings->PlayerStateWidgetClass)
    {
        PlayerStateWidget = CreateWidget<UBaseWidget>(GetWorld(), UISettings->PlayerStateWidgetClass);
    }

    if (UISettings->ResultWidgetClass)
    {
        ResultWidget = CreateWidget<UBaseWidget>(GetWorld(), UISettings->ResultWidgetClass);
    }
}

void UUISubsystem::CloseGame()
{
    if (GetWorld())
    {
        UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);
    }
}