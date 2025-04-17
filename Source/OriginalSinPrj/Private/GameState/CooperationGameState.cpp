// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/CooperationGameState.h"
#include "Kismet/GameplayStatics.h" 
#include "GameMode/CooperationGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Player/Controller/WitchController.h"
#include "../GameInstance/DataSubsystem.h"
#include "OriginalSinPrj/GameInstance/UISubsystem.h"
#include "OriginalSinPrj/GameInstance/EnumSet.h"

#include "OriginalSinPrj/GameInstance/AudioSubsystem.h"
#include "OriginalSinPrj/GameInstance/Struct/CharacterAudioDataStruct.h"
#include "OriginalSinPrj/GameInstance/Struct/BossAudioDataStruct.h"
#include "OriginalSinPrj/GameInstance/Struct/MonsterAudioDataStruct.h"
#include "Components/AudioComponent.h"


ACooperationGameState::ACooperationGameState()
{
    PrimaryActorTick.bCanEverTick = true;

    bIsStage3Started = false;
    SpendedStage3Timer = 0.0f;
    bReplicates = true;

    CameraLocation = FVector(-400.f, 200.f, 0.f);
    CameraRotation = FRotator::ZeroRotator;
    CameraDistance = 0;    
}

void ACooperationGameState::BeginPlay()
{
    Super::BeginPlay();

    // ���� ������Ʈ Ŭ�������� ���� ��忡 ����
    AGameModeBase* GameModeBase = UGameplayStatics::GetGameMode(this);
    CooperationGameGameMode = Cast<ACooperationGameMode>(GameModeBase);
    GameInstance = Cast<UOriginalSinPrjGameInstance>(GetGameInstance());
    UE_LOG(LogTemp, Warning, TEXT("GameState BeginPlay"));


    InitCharacterSounds();
    InitBossSounds();
    InitMonsterSounds();

}
void ACooperationGameState::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (HasAuthority() && bIsStage1Started)
    {
        bIsStage1Reached = true;
        SpendedStage1Timer += DeltaSeconds;
    }
    if (HasAuthority() && bIsStage2Started)
    {
        bIsStage2Reached = true;
        SpendedStage2Timer += DeltaSeconds;
    }
    if (HasAuthority() && bIsStage3Started)
    {
        bIsStage3Reached = true;
        SpendedStage3Timer += DeltaSeconds;
        UpdateTimer();
    }
    if (IsValid(CooperationGameGameMode))
    {
        switch (CooperationGameGameMode->StageIndex)
        {
        case 1:
            SetStage1CameraTransform();

            break;

        case 2:
            SetStage2CameraTransform();

            break;

        case 3:
            //SetStage3CameraTransform(); -> Run only once at Start Stage3  -> in Gamemode
            break;

        default:

            break;
        }
    }
    


}



//ShowLevelWidget


//������ Ÿ�̸� �ѱ�
void ACooperationGameState::TurnOnTimer()
{
    bIsStage3Started = true;
}

void ACooperationGameState::TurnOffTimer()
{
    bIsStage3Started = false;
}


void ACooperationGameState::TurnOnStage1Widget()
{
    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (IsValid(MyGI))
        {
            UE_LOG(LogTemp, Warning, TEXT("GameGI Is ReadyGameGI Is ReadyGameGI Is ReadyGameGI Is ReadyGameGI Is ReadyGameGI Is ReadyGameGI Is ReadyGameGI Is ReadyGameGI Is ReadyGameGI Is ReadyGameGI Is ReadyGameGI Is Ready"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("InValidInValidInValidInValidInValidInValidInValidInValidInValidInValidInValidInValidInValidInValidInValidInValidInValidInValid"));
        }
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            if (IsValid(UISubsystem->CurrentActiveWidget))
            {
                UE_LOG(LogTemp, Warning, TEXT("IValidlilasidladilasiladd"));
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("INVALID"));
            }
            UE_LOG(LogTemp, Warning, TEXT("I::::::::::::%d "), UISubsystem->GetCurrentLevelType());
            
            // ���⼭ UISubsystem ��� ����!
            Cast<UCooperationWidget>(UISubsystem->CurrentActiveWidget)->ActiveStage1Widget();
        }
    }
}
void ACooperationGameState::TurnOnStage2Widget()
{
    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            // ���⼭ UISubsystem ��� ����!
            Cast<UCooperationWidget>(UISubsystem->CurrentActiveWidget)->ActiveStage2Widget();
        }
    }
}
void ACooperationGameState::TurnOnStage3Widget()
{
    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            // ���⼭ UISubsystem ��� ����!
            Cast<UCooperationWidget>(UISubsystem->CurrentActiveWidget)->ActiveStage3Widget();
        }
    }
}

void ACooperationGameState::TurnOffStage3Widget()
{
    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            // ���⼭ UISubsystem ��� ����!
            Cast<UCooperationWidget>(UISubsystem->CurrentActiveWidget)->DeactivateAllWidgets();
        }
    }
}

void ACooperationGameState::CreateBuffSelectUI()
{
    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            UE_LOG(LogTemp, Warning, TEXT("Show Buff Widget"));
            UISubsystem->ShowWidget(EAddWidgetType::BuffSelectWidget);
            UE_LOG(LogTemp, Warning, TEXT("Buff Widget assign?"));
            if (UISubsystem->BuffSelectWidget->IsInViewport())
            {
                TArray<EBuffType> BuffList = BuffUIInit();
                Cast<UBuffSelectWidget>(UISubsystem->BuffSelectWidget)->InitializeBuffs(BuffList);
                UE_LOG(LogTemp, Warning, TEXT("Buff Widget Init"));
            }


            UISubsystem->SetMouseMode(true);
            bIsPlayerBuffSelect = 0;
        }
    }
}



void ACooperationGameState::OnRep_TurnOffBuffUI()
{
    CloseBuffSelectUI();
}


void ACooperationGameState::CloseBuffSelectUI()
{
    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            UE_LOG(LogTemp, Warning, TEXT("Show Buff Widget"));            
            UISubsystem->CloseWidget(EAddWidgetType::BuffSelectWidget);
            UISubsystem->SetMouseMode(false);
        }
    }
}


TArray<EBuffType> ACooperationGameState::BuffUIInit()
{
    //���� ������ ���⼭ ����.
//���� ������ ��ϵǾ��ִ� ����� �޾Ƽ�, ��ġ�� �ʰ� ������ �޾� �������� �ø���

    TArray<EBuffType> BuffArray =
    {
        EBuffType::ManaUp,
        EBuffType::AttackUp,
        EBuffType::CircleUp ,
        EBuffType::LifePointUp,
        EBuffType::DefenseUp,
        EBuffType::AttackSpeedUp,
        EBuffType::AvoidanceUp,
        EBuffType::AttackRangeUp,
        EBuffType::EnemyCircleDown
    };

    TArray<EBuffType> RequestBuffList;
    if (BuffArray.Num() > 0)
    {
        int RandomIndex = FMath::RandRange(0, BuffArray.Num() - 1);
    }

    for (int i = 0; i < 3; i++)
    {
        int32 Index = FMath::RandRange(0, BuffArray.Num() - 1);
        RequestBuffList.Add(BuffArray[Index]);
        BuffArray.RemoveAt(Index);
    }


    return RequestBuffList;
}

void ACooperationGameState::InitCharacterSounds_Implementation()
{
    if (!CheckValidOfAudioHandle())
    {
        return;
    }

    CharacterSounds = AudioHandle->GetCharacterSoundArray();
}

void ACooperationGameState::InitBossSounds_Implementation()
{
    if (!CheckValidOfAudioHandle())
    {
        return;
    }

    BossSounds = AudioHandle->GetBossSoundArray();
}

void ACooperationGameState::InitMonsterSounds_Implementation()
{
    if (!CheckValidOfAudioHandle())
    {
        return;
    }

    MonsterSounds = AudioHandle->GetMonsterSoundArray();
}

void ACooperationGameState::PlayCharacterSound_Implementation(UAudioComponent* AudioComp, ECharacterSoundType SoundType)
{
    if (!CharacterSoundMap.Contains(SoundType))
    {
        if (!LoadCharacterSoundSourceFromArray(SoundType))
        {
            return;
        }
    }

    USoundBase* SoundSource = CharacterSoundMap[SoundType];

    PlaySound(AudioComp, SoundSource);
}

void ACooperationGameState::PlayBossSound_Implementation(UAudioComponent* AudioComp, EBossSoundType SoundType)
{
    if (!BossSoundMap.Contains(SoundType))
    {
        if (!LoadBossSoundSourceFromArray(SoundType))
        {
            return;
        }
    }

    USoundBase* SoundSource = BossSoundMap[SoundType];

    PlaySound(AudioComp, SoundSource);
}

void ACooperationGameState::PlayMonsterSound_Implementation(UAudioComponent* AudioComp, EMonsterSoundType SoundType)
{
    if (!MonsterSoundMap.Contains(SoundType))
    {
        if (!LoadMonsterSoundSourceFromArray(SoundType))
        {
            return;
        }
    }

    USoundBase* SoundSource = MonsterSoundMap[SoundType];

    PlaySound(AudioComp, SoundSource);
}

void ACooperationGameState::PlaySound(UAudioComponent* AudioComp, USoundBase* SoundSource)
{
    if (!IsValid(AudioComp))
    {
        return;
    }

    if (!IsValid(SoundSource))
    {
        return;
    }

    if (!CheckValidOfAudioHandle())
    {
        return;
    }

    float Volume = AudioHandle->GetEffectVolume();

    if (AudioComp->IsPlaying())
    {
        AudioComp->Stop();
    }

    AudioComp->SetVolumeMultiplier(Volume);
    AudioComp->SetSound(SoundSource);
    AudioComp->Play();
}

bool ACooperationGameState::LoadCharacterSoundSourceFromArray(ECharacterSoundType SoundType)
{
    if (CharacterSounds.IsEmpty())
    {
        return false;
    }

    USoundBase* SoundSource = nullptr;

    for (FCharacterAudioDataStruct* SoundData : CharacterSounds)
    {
        if (SoundType == SoundData->CharacterSoundType)
        {
            SoundSource = SoundData->Sound.LoadSynchronous();
            break;
        }
    }

    if (!IsValid(SoundSource))
    {
        return false;
    }

    CharacterSoundMap.Add(SoundType, SoundSource);

    return true;
}

bool ACooperationGameState::LoadBossSoundSourceFromArray(EBossSoundType SoundType)
{
    if (BossSounds.IsEmpty())
    {
        return false;
    }

    USoundBase* SoundSource = nullptr;

    for (FBossAudioDataStruct* SoundData : BossSounds)
    {
        if (SoundType == SoundData->BossSoundType)
        {
            SoundSource = SoundData->Sound.LoadSynchronous();
            break;
        }
    }

    if (!IsValid(SoundSource))
    {
        return false;
    }

    BossSoundMap.Add(SoundType, SoundSource);

    return true;
}

bool ACooperationGameState::LoadMonsterSoundSourceFromArray(EMonsterSoundType SoundType)
{
    if (MonsterSounds.IsEmpty())
    {
        return false;
    }

    USoundBase* SoundSource = nullptr;

    for (FMonsterAudioDataStruct* SoundData : MonsterSounds)
    {
        if (SoundType == SoundData->MonsterSoundType)
        {
            SoundSource = SoundData->Sound.LoadSynchronous();
            break;
        }
    }

    if (!IsValid(SoundSource))
    {
        return false;
    }

    MonsterSoundMap.Add(SoundType, SoundSource);

    return true;
}

bool ACooperationGameState::CheckValidOfAudioHandle()
{
    if (IsValid(AudioHandle))
    {
        return true;
    }

    if (!IsValid(GetGameInstance()))
    {
        return false;
    }

    AudioHandle = GetGameInstance()->GetSubsystem<UAudioSubsystem>();

    if (!IsValid(AudioHandle))
    {
        return false;
    }

    return true;
}



void ACooperationGameState::OnRep_TurnOnStageUI()
{
    if (CurrentStageIndex == 1)
    {
        TurnOnStage1Widget();
    }
    else if (CurrentStageIndex == 2)
    {
        CreateBuffSelectUI();
        TurnOnStage2Widget();
    }
    else if (CurrentStageIndex == 3)
    {
        CreateBuffSelectUI();
        TurnOnStage3Widget();
    }
    else if (CurrentStageIndex == 4)
    {
        TurnOffStage3Widget();
        TurnOnResultWidget();
    }
}

void ACooperationGameState::TurnOnResultWidget()
{
    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            UE_LOG(LogTemp, Warning, TEXT("Show Result Widget"));
            UISubsystem->ShowWidget(EAddWidgetType::ResultWidget);

            if (UISubsystem->ResultWidget->IsInViewport())
            {
                Cast<UResultWidget>(UISubsystem->ResultWidget)->InitResultWidgetData(
                    Player1ReceivedDamage,
                    Player2ReceivedDamage,
                    Player1DeathCount,
                    Player2DeathCount,
                    Player1ApplyAttackCount,
                    Player2ApplyAttackCount,
                    SpendedStage1Timer,
                    SpendedStage2Timer,
                    SpendedStage3Timer,
                    bIsStage1Reached,
                    bIsStage2Reached,
                    bIsStage3Reached);
            }

            UISubsystem->SetMouseMode(true);
            // ���⼭ UISubsystem ��� ����!
            //Cast<UCooperationWidget>(UISubsystem->CurrentActiveWidget)->ActiveStage2Widget();
        }
    }
}

void ACooperationGameState::InitPlayerInfo()
{
    if (HasAuthority())  // ���������� ����Ǵ� �ڵ�
    {
        PlayerInfos.Empty(); // ���� ���� ����
        //��𼱰� �޾ƿ;���...
        FCharacterStateBuffer Player1State;
        FCharacterStateBuffer Player2State;

        PlayerInfos.Add(CooperationGameGameMode->ActivePlayers[0], FPlayerData{
        "Player1", nullptr,
        100.0f, 100.0f,
        0, 0,
        0, 0.0f,
        0,
        0,
        0, 100, 1 });
        Player1StateData = PlayerInfos[CooperationGameGameMode->ActivePlayers[0]];

        PlayerInfos.Add(CooperationGameGameMode->ActivePlayers[1], FPlayerData{
        "Player2", nullptr,
        100.0f, 100.0f,
        0, 0,
        0, 0.0f,
        0,
        0,
        0, 100, 1 });
        Player2StateData = PlayerInfos[CooperationGameGameMode->ActivePlayers[1]];
        InitPlayerUIInfo();

        Cast<ABaseWitch>(CooperationGameGameMode->ActivePlayers[0])->ResetCharacterState();
        Cast<ABaseWitch>(CooperationGameGameMode->ActivePlayers[1])->ResetCharacterState();
    }
}

void ACooperationGameState::UpdatePlayerInfo(const FCharacterStateBuffer& State)
{  

    UE_LOG(LogTemp, Warning, TEXT("Update Info"));
    
    if (State.OwnWitch == CooperationGameGameMode->ActivePlayers[0])
    {
        if (PlayerInfos[CooperationGameGameMode->ActivePlayers[0]].CurrentHP != State.CurrentHP)
        {
            UE_LOG(LogTemp, Warning, TEXT("Player1 HP Updated %f"), (PlayerInfos[CooperationGameGameMode->ActivePlayers[0]].CurrentHP - State.CurrentHP));
            Player1ReceivedDamage += (PlayerInfos[CooperationGameGameMode->ActivePlayers[0]].CurrentHP - State.CurrentHP);

            PlayerInfos[CooperationGameGameMode->ActivePlayers[0]].CurrentHP = State.CurrentHP;
            
        }
        PlayerInfos[CooperationGameGameMode->ActivePlayers[0]].CurrentEXP = State.CurrentEXP;
        
        if (PlayerInfos[CooperationGameGameMode->ActivePlayers[0]].CurrentMana != State.CurrentMana)
        {
            Player1ApplyAttackCount++;
            PlayerInfos[CooperationGameGameMode->ActivePlayers[0]].CurrentMana = State.CurrentMana;
        }
        
        PlayerInfos[CooperationGameGameMode->ActivePlayers[0]].MaxHP = State.MaxHP;
        PlayerInfos[CooperationGameGameMode->ActivePlayers[0]].MaxEXP = State.MaxEXP;
        PlayerInfos[CooperationGameGameMode->ActivePlayers[0]].MaxMana = State.MaxMana;
        PlayerInfos[CooperationGameGameMode->ActivePlayers[0]].PlayerLevel = State.PlayerLevel;
        PlayerInfos[CooperationGameGameMode->ActivePlayers[0]].LifePoint = State.LifePoint;
        PlayerInfos[CooperationGameGameMode->ActivePlayers[0]].AirbornePercent = State.AirbornePercent;
        Player1StateData = PlayerInfos[CooperationGameGameMode->ActivePlayers[0]];
        CooperationGameGameMode->RequestUpdateUI(0);
        
    }
    
    
    
    if (State.OwnWitch == CooperationGameGameMode->ActivePlayers[1])
    {
        if (PlayerInfos[CooperationGameGameMode->ActivePlayers[1]].CurrentHP != State.CurrentHP)
        {
            UE_LOG(LogTemp, Warning, TEXT("Player1 HP Updated %f"), (PlayerInfos[CooperationGameGameMode->ActivePlayers[1]].CurrentHP - State.CurrentHP));
            Player2ReceivedDamage += (PlayerInfos[CooperationGameGameMode->ActivePlayers[1]].CurrentHP - State.CurrentHP);
            PlayerInfos[CooperationGameGameMode->ActivePlayers[1]].CurrentHP = State.CurrentHP;
        }
        PlayerInfos[CooperationGameGameMode->ActivePlayers[1]].CurrentEXP = State.CurrentEXP;
        
        if (PlayerInfos[CooperationGameGameMode->ActivePlayers[1]].CurrentMana != State.CurrentMana)
        {
            Player2ApplyAttackCount++;
            PlayerInfos[CooperationGameGameMode->ActivePlayers[1]].CurrentMana = State.CurrentMana;
        }

        PlayerInfos[CooperationGameGameMode->ActivePlayers[1]].MaxHP = State.MaxHP;
        PlayerInfos[CooperationGameGameMode->ActivePlayers[1]].MaxEXP = State.MaxEXP;
        PlayerInfos[CooperationGameGameMode->ActivePlayers[1]].MaxMana = State.MaxMana;
        PlayerInfos[CooperationGameGameMode->ActivePlayers[1]].PlayerLevel = State.PlayerLevel;
        PlayerInfos[CooperationGameGameMode->ActivePlayers[1]].LifePoint = State.LifePoint;
        PlayerInfos[CooperationGameGameMode->ActivePlayers[1]].AirbornePercent = State.AirbornePercent;
        Player2StateData = PlayerInfos[CooperationGameGameMode->ActivePlayers[1]];
        CooperationGameGameMode->RequestUpdateUI(1);
    }
}


void ACooperationGameState::InitPlayerUIInfo()
{
    //���� �����ؼ� ����
    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            // ���⼭ UISubsystem ��� ����!
            Cast<UCooperationWidget>(UISubsystem->CurrentActiveWidget)->InitPlayerUI(&Player1StateData, &Player2StateData);
        }
    }
}

void ACooperationGameState::UpdatePlayerUIInfo()
{
    //���� �����ؼ� ����
    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            // ���⼭ UISubsystem ��� ����!
            Cast<UCooperationWidget>(UISubsystem->CurrentActiveWidget)->UpdatePlayerUI(&Player1StateData, &Player2StateData);
        }
    }
}


void ACooperationGameState::ReceiveSelectedBuff(APlayerController* player, FBuffType* Bufftype)
{
    if (IsValid(CooperationGameGameMode))
    {
        if (CooperationGameGameMode->StageIndex == 2)
        {

        }
        else if (CooperationGameGameMode->StageIndex == 3)
        {

        }
    }

    if (player == PlayerControllerSet[0]) //<- �̰� �³�?
    {
        Player1Stage1SelectedBuff = Bufftype;

    }

    if (player == PlayerControllerSet[1])
    {
        Player2Stage1SelectedBuff = Bufftype;
    }

    
}

void ACooperationGameState::ApplyBuffStat()
{
    if (IsValid(CooperationGameGameMode))
    {
        if (CooperationGameGameMode->StageIndex == 2)
        {
            /*
            PlayerInfo += 
            SelectedBuff[0];
            SelectedBuff[1];
            */
        }
        else if (CooperationGameGameMode->StageIndex == 3)
        {
            /*
            PlayerInfo += 
            SelectedBuff[2];
            SelectedBuff[3];
            */
        }
    }
    //UI ���� ���

    bIsPlayerBuffSelect = 0;
}











void ACooperationGameState::AddExperienceToPlayer(AActor* Player, int32 Amount)
{
    if (Player)
    {
        // PlayerInfos���� �÷��̾� ���� ã��
        FPlayerData* PlayerData = PlayerInfos.Find(Player);

        if (PlayerData)
        {
            PlayerData->CurrentEXP += Amount;
            CheckLevelUp(Player);
            UpdatePlayerUIInfo(); // UI ����
        }
    }
}

void ACooperationGameState::CheckLevelUp(AActor* Player)
{
    if (Player)
    {
        FPlayerData& Info = PlayerInfos[Player];

        while (Info.CurrentEXP >= 100)
        {
            Info.CurrentEXP -= 100;
            Info.PlayerLevel++;

        }
    }
}


void ACooperationGameState::SetPlayerPawn(ABaseWitch* InPawn)
{
    PlayerPawnRef = InPawn;
}

//���� ��Ʈ�ѷ� ����صα�
void ACooperationGameState::RegisterInitialController(APlayerController* PC)
{
    if (IsValid(PC) && !PlayerControllerSet.Contains(PC))
    {
        PlayerControllerSet.Add(PC);
        UE_LOG(LogTemp, Log, TEXT("Controller Registered: %s"), *PC->GetName());
    }
}



void ACooperationGameState::SetStage1CameraTransform()
{
    FVector SumPlayerLocation = FVector::ZeroVector;
    float minY = 99999.f;
    float minZ = 99999.f;

    float maxY = -99999.f;
    float maxZ = -99999.f;


    for (AActor* Player : CooperationGameGameMode->AlivePlayers)
    {
        FVector PlayerLocation = Player->GetActorLocation();

        SumPlayerLocation += PlayerLocation;
        if (PlayerLocation.Y > maxY)
        {
            maxY = PlayerLocation.Y;
        }
        if (PlayerLocation.Y < minY)
        {
            minY = PlayerLocation.Y;
        }

        if (PlayerLocation.Z > maxZ)
        {
            maxZ = PlayerLocation.Z;
        }
        if (PlayerLocation.Z < minZ)
        {
            minZ = PlayerLocation.Z;
        }
    }

    FVector MeanPlayerLocation = FVector::ZeroVector;
    if (CooperationGameGameMode->AlivePlayers.Num() > 0)
    {
        MeanPlayerLocation = SumPlayerLocation / CooperationGameGameMode->AlivePlayers.Num();
    }

    if (HasAuthority()) // �������� Ȯ��
    {
        CameraDistance = maxY - minY + 2 * (maxZ - minZ);

        CameraLocation = MeanPlayerLocation;
        CameraRotation = FRotator::ZeroRotator;
    }

    CooperationGameGameMode->SpawnedBaseCamera[0]->UpdateCameraLocationandRotation();
    
}

void ACooperationGameState::SetStage2CameraTransform()
{
    FVector SumPlayerLocation = FVector::ZeroVector;
    FVector SumEnemyLocation = FVector::ZeroVector;

    float minY = 99999.f;
    float minZ = 99999.f;

    float maxY = -99999.f;
    float maxZ = -99999.f;

    for (AActor* Player : CooperationGameGameMode->AlivePlayers)
    {
        FVector PlayerLocation = Player->GetActorLocation();

        SumPlayerLocation += PlayerLocation;
        if (PlayerLocation.Y > maxY)
        {
            maxY = PlayerLocation.Y;
        }
        if (PlayerLocation.Y < minY)
        {
            minY = PlayerLocation.Y;
        }

        if (PlayerLocation.Z > maxZ)
        {
            maxZ = PlayerLocation.Z;
        }
        if (PlayerLocation.Z < minZ)
        {
            minZ = PlayerLocation.Z;
        }
    }

    for (AActor* Enemy : CooperationGameGameMode->ActiveEnemies)
    {
        if (IsValid(Enemy))
        {
            FVector EnemyLocation = Enemy->GetActorLocation();

            SumEnemyLocation += EnemyLocation;

            if (EnemyLocation.Y > maxY)
            {
                maxY = EnemyLocation.Y;
            }
            if (EnemyLocation.Y < minY)
            {
                minY = EnemyLocation.Y;
            }

            if (EnemyLocation.Z > maxZ)
            {
                maxZ = EnemyLocation.Z;
            }
            if (EnemyLocation.Z < minZ)
            {
                minZ = EnemyLocation.Z;
            }
        }
    }

    FVector MeanActorLocation = FVector::ZeroVector;

    int32 NumOfActors = CooperationGameGameMode->AlivePlayers.Num() + CooperationGameGameMode->ActiveEnemies.Num();

    if (CooperationGameGameMode->AlivePlayers.Num() + CooperationGameGameMode->ActiveEnemies.Num() > 0)
    {
        MeanActorLocation = (SumPlayerLocation + SumEnemyLocation) / NumOfActors;
    }


    if (HasAuthority())
    {
        CameraDistance = maxY - minY + 2 * (maxZ - minZ);
        CameraLocation = MeanActorLocation;
        CameraRotation = FRotator::ZeroRotator;

    }// �������� Ȯ��

    CooperationGameGameMode->SpawnedBaseCamera[0]->UpdateCameraLocationandRotation();
}

void ACooperationGameState::SetStage3CameraTransform()
{

    //Fix Camera Settings
    CameraDistance = CooperationGameGameMode->BossStageCameraDistance[0];
    CameraLocation = CooperationGameGameMode->BossStageCameraLocations[0];
    CameraRotation = CooperationGameGameMode->BossStageCameraAngle[0];

    CooperationGameGameMode->SpawnedBaseCamera[0]->UpdateCameraLocationandRotation();
}


//�������� ��ġ���Ͽ� ���� ����

//








////////////////////////////��Ƽ ó��

void ACooperationGameState::OnRep_UpdatePlayer1DataUI()
{
    UpdatePlayerUIInfo();
}

void ACooperationGameState::OnRep_UpdatePlayer2DataUI()
{
    UpdatePlayerUIInfo();
}


void ACooperationGameState::OnRep_UpdatePlayerInitData()
{
    InitPlayerUIInfo();
}


void ACooperationGameState::UpdateTimer()
{
    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            // ���⼭ UISubsystem ��� ����!
            Cast<UCooperationWidget>(UISubsystem->CurrentActiveWidget)->UpdateBossTimer(SpendedStage3Timer);
        }
    }
}

void ACooperationGameState::OnRep_UpdateTimer()
{
    UE_LOG(LogTemp, Warning, TEXT("Timer: %f"), SpendedStage3Timer);
    UpdateTimer();
}



void ACooperationGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACooperationGameState, CameraLocation);
    DOREPLIFETIME(ACooperationGameState, CameraRotation);
    DOREPLIFETIME(ACooperationGameState, CameraDistance);
    DOREPLIFETIME(ACooperationGameState, SpendedStage3Timer);
    DOREPLIFETIME(ACooperationGameState, bIsPlayerCanMove); 
    DOREPLIFETIME(ACooperationGameState, CurrentStageIndex);
    DOREPLIFETIME(ACooperationGameState, bIsPlayerBuffSelect);
    DOREPLIFETIME(ACooperationGameState, SelectBuffPlayer);
    DOREPLIFETIME(ACooperationGameState, Player1DataChanged);
    DOREPLIFETIME(ACooperationGameState, Player2DataChanged);
    DOREPLIFETIME(ACooperationGameState, Player1StateData);
    DOREPLIFETIME(ACooperationGameState, Player2StateData);
    DOREPLIFETIME(ACooperationGameState, PlayerDataChanged);


    DOREPLIFETIME(ACooperationGameState, Player1ReceivedDamage);
    DOREPLIFETIME(ACooperationGameState, Player2ReceivedDamage);
    DOREPLIFETIME(ACooperationGameState, Player1DeathCount);
    DOREPLIFETIME(ACooperationGameState, Player2DeathCount);
    DOREPLIFETIME(ACooperationGameState, Player1ApplyAttackCount);
    DOREPLIFETIME(ACooperationGameState, Player2ApplyAttackCount);
    DOREPLIFETIME(ACooperationGameState, SpendedStage1Timer);
    DOREPLIFETIME(ACooperationGameState, SpendedStage2Timer);
    DOREPLIFETIME(ACooperationGameState, SpendedStage2Timer);
    DOREPLIFETIME(ACooperationGameState, bIsStage1Reached);
    DOREPLIFETIME(ACooperationGameState, bIsStage2Reached);
    DOREPLIFETIME(ACooperationGameState, bIsStage3Reached);
    DOREPLIFETIME(ACooperationGameState, bIsStage3Reached);
    DOREPLIFETIME(ACooperationGameState, bIsStage1Started);
    DOREPLIFETIME(ACooperationGameState, bIsStage2Started);
    DOREPLIFETIME(ACooperationGameState, bIsStage3Started);

}


void ACooperationGameState::OnRep_SetPlayerMove()
{
    UE_LOG(LogTemp, Log, TEXT("This is NOT the local player controller."));
    UWorld* WorldContext = GetWorld();
    for (FConstPlayerControllerIterator It = WorldContext->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();
        if (PC && PC->GetPawn())
        {
            if (bIsPlayerCanMove)
            {
                PC->GetPawn()->DisableInput(PC);
            }
            else
            {
                PC->GetPawn()->EnableInput(PC);
            }
        }
    }
    
}


void ACooperationGameState::SetPlayerMove(bool bCanMove)
{
    bIsPlayerCanMove = bCanMove;

    if (bIsPlayerCanMove)
    {
        bIsPlayerCanMove = false;
        UWorld* WorldContext = GetWorld();

        for (FConstPlayerControllerIterator It = WorldContext->GetPlayerControllerIterator(); It; ++It)
        {
            APlayerController* PC = It->Get();
            if (PC && PC->GetPawn())
            {
                PC->GetPawn()->DisableInput(PC);
            }
        }
    }
    else
    {
        bIsPlayerCanMove = true;
        UWorld* WorldContext = GetWorld();

        for (FConstPlayerControllerIterator It = WorldContext->GetPlayerControllerIterator(); It; ++It)
        {
            APlayerController* PC = It->Get();
            if (PC && PC->GetPawn())
            {
                PC->GetPawn()->EnableInput(PC);
            }
        }
    }
}


void ACooperationGameState::OnRep_UpdateBossDataUI()
{
    UpdateBossDataUI();
}

void ACooperationGameState::UpdateBossDataUI()
{
    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            // ���⼭ UISubsystem ��� ����!
            Cast<UCooperationWidget>(UISubsystem->CurrentActiveWidget)->UpdateBossUI(BossData[0]);
        }
    }
}


void ACooperationGameState::ApplyDamage(AActor* Attacker, float Damage, const FVector& HitLocation)
{
    Multicast_ApplyDamage(Attacker, Damage, HitLocation);
}

void ACooperationGameState::TakeDamage(AActor* Victim, float Damage, const FVector& HitLocation) 
{
    Multicast_TakeDamage(Victim, Damage, HitLocation);
}

void ACooperationGameState::OnDeathPlayer(ACharacter* Player, const FVector& DeathLocation) 
{
    ABaseWitch* DeadPlayer = Cast<ABaseWitch>(Player);
    if (DeadPlayer == CooperationGameGameMode->ActivePlayers[0])
    {
        Player1DeathCount++;
    }
    else if (DeadPlayer == CooperationGameGameMode->ActivePlayers[1])
    {
        Player2DeathCount++;
    }
}

void ACooperationGameState::OnDeathMonster(AActor* Monster, const FVector& DeathLocation) 
{
    Multicast_OnDeathMonster(Monster, DeathLocation);
}



void ACooperationGameState::Multicast_ApplyDamage_Implementation(AActor* Attacker, float Damage, const FVector& HitLocation)
{
    // ��� Ŭ���̾�Ʈ���� �����
    if (IsValid(Attacker))
    {
        // ��: �ǰ� ��ġ�� ����Ʈ ����
        // UGameplayStatics::SpawnEmitterAtLocation(...);
    }

    // ���� ������ ó���� �ʿ��ϴٸ� ���⼭ ���� (�Ǵ� ���������� �� ���� ����)
}

void ACooperationGameState::Multicast_TakeDamage_Implementation(AActor* Victim, float Damage, const FVector& HitLocation)
{
    if (IsValid(Victim))
    {
        // ��: ���� ���� �ִϸ��̼� ���
    }
   
    /*
    Victim.HP -= Damage;
    If(Victim.HP <= MaxHP)
    {
        if(Victim == Player)
            Multicast_OnDeathPlayer_Implementation(Victim, HitLocation);
        if Victim == enemy
            Multicast_OnDeathPlayer_Implementation(Victim, HitLocation);
        if Victim == monster
            Multicast_OnDeathMonster_Implementation(Victim, HitLocation);
    }

    */
}

void ACooperationGameState::Multicast_OnDeathPlayer_Implementation(ACharacter* Player, const FVector& DeathLocation)
{
    if (IsValid(Player))
    {
        /*
        if (Victim == Player)
            Multicast_OnDeathPlayer_Implementation(Victim, HitLocation);
            playDeathAnim
            if Victim.Life > 0
                Respawn
            else
                HandlePlayerKilled

        if Victim == enemy
            Multicast_OnDeathPlayer_Implementation(Victim, HitLocation);
            GameMode->HandleEnemyKilled(Monster, AController* Killer)
        */

        // ��: ��� �ִϸ��̼�, ȿ�� ��
        // Player->PlayDeathAnimation(); ���� �Լ� ȣ��
    }
}

void ACooperationGameState::Multicast_OnDeathMonster_Implementation(AActor* Monster, const FVector& DeathLocation)
{
    if (IsValid(Monster))
    {
        /*
        

        if monster?
        GameMode->HandleMonsterKilled(Monster, AController* Killer)



        */
        

    }
}
