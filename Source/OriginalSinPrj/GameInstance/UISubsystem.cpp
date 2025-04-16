// UISubsystem.cpp

#include "UISubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "../Widget/BaseWidget.h"
#include "UISettings.h"
#include "AudioSubsystem.h"
#include "OriginalSinPrj/GameInstance/OriginalSinPrjGameInstance.h"

void UUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    //World = GetGameInstance()->GetWorld();

    UISettings = GetDefault<UUISettings>();

    //CreateWidgets();
}

void UUISubsystem::ShowLevelWidget(ELevelType LevelType)
{
    UE_LOG(LogTemp, Warning, TEXT("Show Level Widget"));
    switch (LevelType)
    {
    case ELevelType::IntroLevel:
        if (!IsValid(IntroLevelWidget))
        {
            IntroLevelWidget = CreateWidgetByClass(UISettings->IntroWidgetClass);
        }
        CurrentActiveWidget = IntroLevelWidget;
        break;

    case ELevelType::TitleLevel:
        if (!IsValid(TitleLevelWidget))
        {
            TitleLevelWidget = CreateWidgetByClass(UISettings->TitleWidgetClass);
        }
        CurrentActiveWidget = TitleLevelWidget;
        break;

    case ELevelType::MatchLevel:
        if (!IsValid(MatchLevelWidget))
        {
            MatchLevelWidget = CreateWidgetByClass(UISettings->MatchWidgetClass);
        }
        CurrentActiveWidget = MatchLevelWidget;
        break;

    case ELevelType::LobbyLevel:
        if (!IsValid(LobbyLevelWidget))
        {
            LobbyLevelWidget = CreateWidgetByClass(UISettings->LobbyWidgetClass);
        }
        CurrentActiveWidget = LobbyLevelWidget;
        break;

    case ELevelType::MultiLobbyLevel:
        if (!IsValid(MultiLobbyLevelWidget))
        {
            MultiLobbyLevelWidget = CreateWidgetByClass(UISettings->MultiLobbyWidgetClass);
        }
        CurrentActiveWidget = MultiLobbyLevelWidget;
        break;

    case ELevelType::MultiLevel:
        if (!IsValid(MultiLevelWidget))
        {
            MultiLevelWidget = CreateWidgetByClass(UISettings->MultiWidgetClass);
        }
        CurrentActiveWidget = MultiLevelWidget;
        break;

    case ELevelType::CooperationLevel:
        if (!IsValid(CooperationLevelWidget))
        {
            CooperationLevelWidget = CreateWidgetByClass(UISettings->CooperationWidgetClass);
        }
        CurrentActiveWidget = CooperationLevelWidget;
        break;

    case ELevelType::SingleLevel:
        if (!IsValid(SingleLevelWidget))
        {
            SingleLevelWidget = CreateWidgetByClass(UISettings->SingleWidgetClass);
        }
        CurrentActiveWidget = SingleLevelWidget;
        break;

    case ELevelType::TrainingLevel:
        if (!IsValid(TrainingLevelWidget))
        {
            TrainingLevelWidget = CreateWidgetByClass(UISettings->TrainingWidgetClass);
        }
        CurrentActiveWidget = TrainingLevelWidget;
        break;

    case ELevelType::FarmingLevel:
        if (!IsValid(FarmingLevelWidget))
        {
            FarmingLevelWidget = CreateWidgetByClass(UISettings->FarmingWidgetClass);
        }
        CurrentActiveWidget = FarmingLevelWidget;
        break;

    default:
        checkNoEntry();
        break;
    }

    if (IsValid(CurrentActiveWidget))
    {
        CurrentActiveWidget->AddToViewport();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Current Active Widget is invalid"));
    }
}


void UUISubsystem::OnPostLoadMap(UWorld* LoadedWorld)
{
    FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this); // ���� ��� �̺�Ʈ ���� ����

    if (CurrentActiveWidget)
    {
        CurrentActiveWidget->AddToViewport();
        
        if (UAudioSubsystem* AudioSubsystem = GetGameInstance()->GetSubsystem<UAudioSubsystem>())
        {
            // BGM ��� ȣ�� ���� ������ �ʿ��� ���� ����, ���� ��ȯ�� ������ ����Ǵ� BGM�� ���� ���ư�
            if (CurrentActiveWidget == IntroLevelWidget)
            {
                AudioSubsystem->PlayBGM(ELevelSoundType::TitleSound);
            }

            // �ʿ� ������ ���..
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid CurrentActiveWidget"));
    }
}

UBaseWidget* UUISubsystem::CreateWidgetByClass(TSubclassOf<UBaseWidget> WidgetClass)
{
    if (!IsValid(WidgetClass))
    {
        UE_LOG(LogTemp, Warning, TEXT("Widget Class is invali"));
        return nullptr;
    }

    TempWidget = CreateWidget<UBaseWidget>(GetGameInstance(), WidgetClass);
    TempWidget->InitWidget(this);
    return TempWidget;
}

void UUISubsystem::ShowWidget(EAddWidgetType WidgetType)
{
    UBaseWidget* TargetWidget = nullptr;

    switch (WidgetType)
    {
        case EAddWidgetType::OptionWidget:
            if (!IsValid(OptionWidget))
            {
                OptionWidget = CreateWidgetByClass(UISettings->OptionWidgetClass);
            }
            TargetWidget = OptionWidget; 
            break;

        case EAddWidgetType::BuffSelectWidget:
            if (!IsValid(BuffSelectWidget))
            {
                BuffSelectWidget = CreateWidgetByClass(UISettings->BuffSelectWidgetClass);
            }
            TargetWidget = BuffSelectWidget;
            break;

        case EAddWidgetType::CharacterSelectWidget:
            if (!IsValid(CharacterSelectWidget))
            {
                CharacterSelectWidget = CreateWidgetByClass(UISettings->CharacterSelectWidgetClass);
            }
            TargetWidget = CharacterSelectWidget;
            break;

        case EAddWidgetType::GameSettingWidget:
            if (!IsValid(GameSettingWidget))
            {
                GameSettingWidget = CreateWidgetByClass(UISettings->GameSettingWidgetClass);
            }
            TargetWidget = GameSettingWidget;
            break;

        case EAddWidgetType::MapSelectWidget:
            if (!IsValid(MapSelectWidget))
            {
                MapSelectWidget = CreateWidgetByClass(UISettings->MapSelectWidgetClass);
            }
            TargetWidget = MapSelectWidget;
            break;

        case EAddWidgetType::PlayerStateWidget:
            if (!IsValid(PlayerStateWidget))
            {
                PlayerStateWidget = CreateWidgetByClass(UISettings->PlayerStateWidgetClass);
            }
            TargetWidget = PlayerStateWidget;
            break;

        case EAddWidgetType::ResultWidget:
            if (!IsValid(ResultWidget))
            {
                ResultWidget = CreateWidgetByClass(UISettings->ResultWidgetClass);
            }
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
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid TargetWidget"));
    }
}

void UUISubsystem::CloseWidget(EAddWidgetType WidgetType)
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

    if (TargetWidget->IsInViewport())
    {
        TargetWidget->RemoveFromParent();
    }
}


void UUISubsystem::CreateWidgets()
{
    if (UISettings->IntroWidgetClass)
    {
        IntroLevelWidget = CreateWidget<UBaseWidget>(GetWorld(), UISettings->IntroWidgetClass);
        IntroLevelWidget->InitWidget(this);
    }

    if (UISettings->TitleWidgetClass)
    {
        TitleLevelWidget = CreateWidget<UBaseWidget>(GetWorld(), UISettings->TitleWidgetClass);
        TitleLevelWidget->InitWidget(this);
    }

    if (UISettings->MatchWidgetClass)
    {
        MatchLevelWidget = CreateWidget<UBaseWidget>(GetWorld(), UISettings->MatchWidgetClass);
        MatchLevelWidget->InitWidget(this);
    }

    if (UISettings->LobbyWidgetClass)
    {
        LobbyLevelWidget = CreateWidget<UBaseWidget>(GetWorld(), UISettings->LobbyWidgetClass);
        LobbyLevelWidget->InitWidget(this);
    }

    if (UISettings->MultiLobbyWidgetClass)
    {
        MultiLobbyLevelWidget = CreateWidget<UBaseWidget>(GetWorld(), UISettings->MultiLobbyWidgetClass);
        MultiLobbyLevelWidget->InitWidget(this);
    }

    if (UISettings->MultiWidgetClass)
    {
        MultiLevelWidget = CreateWidget<UBaseWidget>(GetWorld(), UISettings->MultiWidgetClass);
        MultiLevelWidget->InitWidget(this);
    }

    if (UISettings->CooperationWidgetClass)
    {
        CooperationLevelWidget = CreateWidget<UBaseWidget>(GetWorld(), UISettings->CooperationWidgetClass);
        CooperationLevelWidget->InitWidget(this);
    }

    if (UISettings->SingleWidgetClass)
    {
        SingleLevelWidget = CreateWidget<UBaseWidget>(GetWorld(), UISettings->SingleWidgetClass);
        SingleLevelWidget->InitWidget(this);
    }

    if (UISettings->TrainingWidgetClass)
    {
        TrainingLevelWidget = CreateWidget<UBaseWidget>(GetWorld(), UISettings->TrainingWidgetClass);
        TrainingLevelWidget->InitWidget(this);
    }

    if (UISettings->FarmingWidgetClass)
    {
        FarmingLevelWidget = CreateWidget<UBaseWidget>(GetWorld(), UISettings->FarmingWidgetClass);
        FarmingLevelWidget->InitWidget(this);
    }

    // On/Off Widget

    if (UISettings->OptionWidgetClass)
    {
        OptionWidget = CreateWidget<UBaseWidget>(GetWorld(), UISettings->OptionWidgetClass);
        OptionWidget->InitWidget(this);
    }

    if (UISettings->BuffSelectWidgetClass)
    {
        BuffSelectWidget = CreateWidget<UBaseWidget>(GetWorld(), UISettings->BuffSelectWidgetClass);
        BuffSelectWidget->InitWidget(this);
    }

    if (UISettings->CharacterSelectWidgetClass)
    {
        CharacterSelectWidget = CreateWidget<UBaseWidget>(GetWorld(), UISettings->CharacterSelectWidgetClass);
        CharacterSelectWidget->InitWidget(this);
    }

    if (UISettings->GameSettingWidgetClass)
    {
        GameSettingWidget = CreateWidget<UBaseWidget>(GetWorld(), UISettings->GameSettingWidgetClass);
        GameSettingWidget->InitWidget(this);
    }

    if (UISettings->MapSelectWidgetClass)
    {
        MapSelectWidget = CreateWidget<UBaseWidget>(GetWorld(), UISettings->MapSelectWidgetClass);
        MapSelectWidget->InitWidget(this);
    }

    if (UISettings->PlayerStateWidgetClass)
    {
        PlayerStateWidget = CreateWidget<UBaseWidget>(GetWorld(), UISettings->PlayerStateWidgetClass);
        PlayerStateWidget->InitWidget(this);
    }

    if (UISettings->ResultWidgetClass)
    {
        ResultWidget = CreateWidget<UBaseWidget>(GetWorld(), UISettings->ResultWidgetClass);
        ResultWidget->InitWidget(this);
    }
}

void UUISubsystem::SetVisibilityWidget(bool bIsVisible)
{
    CurrentActiveWidget->SetWidgetVisibility(bIsVisible);
}

const ELevelType UUISubsystem::GetCurrentLevelType()
{
    if (CheckValidOfGameInstance())
    {
        CurrentLevel = GameInstance->GetCurrentLevelType();
    }

    return CurrentLevel;
}

void UUISubsystem::OnClickedMoveLevel(ELevelType LevelType, bool bIsSingle)
{
    if (!CheckValidOfGameInstance())
    {
        return;
    }

    GameInstance->RequestOpenLevelByType(LevelType, bIsSingle);
}

bool UUISubsystem::CheckValidOfGameInstance()
{
    if (IsValid(GameInstance))
    {
        return true;
    }

    if (!IsValid(GetGameInstance()))
    {
        return false;
    }

    GameInstance = Cast<UOriginalSinPrjGameInstance>(GetGameInstance());

    if (!IsValid(GameInstance))
    {
        return false;
    }

    return true;
}

void UUISubsystem::CloseGame()
{
    if (GetWorld())
    {
        UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);
    }
}

void UUISubsystem::SetMouseMode(bool MouseMode)
{
    if (!CheckValidOfGameInstance())
    {
        return;
    }

    APlayerController* PC = GameInstance->GetOwningPlayerController();

    if (!IsValid(PC))
    {
        return;
    }

    if (MouseMode)
    {
        PC->bShowMouseCursor = true;
        FInputModeUIOnly InputMode;
        InputMode.SetWidgetToFocus(CurrentActiveWidget->TakeWidget());
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        PC->SetInputMode(InputMode);
        UE_LOG(LogTemp, Warning, TEXT("MouseMode On"));
    }
    else
    {
        FInputModeGameOnly InputMode;
        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = false;
        UE_LOG(LogTemp, Warning, TEXT("MouseMode Off"));
    }
}