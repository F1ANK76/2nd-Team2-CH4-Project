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
