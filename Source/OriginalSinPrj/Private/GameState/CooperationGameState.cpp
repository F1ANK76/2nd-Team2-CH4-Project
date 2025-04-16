// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/CooperationGameState.h"
#include "Kismet/GameplayStatics.h" 
#include "GameMode/CooperationGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Player/Controller/WitchController.h"
#include "../GameInstance/DataSubsystem.h"
#include "OriginalSinPrj/GameInstance/UISubsystem.h"


ACooperationGameState::ACooperationGameState()
{
    PrimaryActorTick.bCanEverTick = true;
    SetActorTickEnabled(false); // 일단 꺼두기

    bIsStage3Started = false;
    Timer = 0.0f;
    bReplicates = true;

    CameraLocation = FVector(-400.f, 200.f, 0.f);
    CameraRotation = FRotator::ZeroRotator;
    CameraDistance = 0;    
}

void ACooperationGameState::BeginPlay()
{
    Super::BeginPlay();
    
    // 게임 스테이트 클래스에서 게임 모드에 접근
    AGameModeBase* GameModeBase = UGameplayStatics::GetGameMode(this);
    CooperationGameGameMode = Cast<ACooperationGameMode>(GameModeBase);
    GameInstance = Cast<UOriginalSinPrjGameInstance>(GetGameInstance());
    UE_LOG(LogTemp, Warning, TEXT("GameState BeginPlay"));
}
void ACooperationGameState::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

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
    

    if (HasAuthority() && bIsStage3Started)
    {
        Timer += DeltaSeconds;
        UpdateTimer();
    }
}



//ShowLevelWidget


//보스전 타이머 켜기
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
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            // 여기서 UISubsystem 사용 가능!
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
            // 여기서 UISubsystem 사용 가능!
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
            // 여기서 UISubsystem 사용 가능!
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
            // 여기서 UISubsystem 사용 가능!
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
            // 여기서 UISubsystem 사용 가능!
            //Cast<UCooperationWidget>(UISubsystem->CurrentActiveWidget)->ActiveStage2Widget();
        }
    }
}

void ACooperationGameState::InitPlayerInfo()
{
    if (HasAuthority())  // 서버에서만 실행되는 코드
    {
        PlayerInfos.Empty(); // 기존 정보 삭제
        //어디선가 받아와야해...
        FCharacterStateBuffer Player1State;
        FCharacterStateBuffer Player2State;

        PlayerInfos.Add(CooperationGameGameMode->ActivePlayers[0], FPlayerData{
        "Player1", nullptr, 
        100.0f, 100.0f, 
        0, 0, 
        0, 0.0f, 
        0, 
        0, 
        0, 100 });
        Player1StateData = PlayerInfos[CooperationGameGameMode->ActivePlayers[0]];
        PlayerInfos.Add(CooperationGameGameMode->ActivePlayers[1], FPlayerData{
        "Player2", nullptr,
        100.0f, 100.0f,
        0, 0,
        0, 0.0f,
        0,
        0,
        0, 100 });
        Player2StateData = PlayerInfos[CooperationGameGameMode->ActivePlayers[1]];
    }
    InitPlayerUIInfo();

}

void ACooperationGameState::UpdatePlayerInfo(const FCharacterStateBuffer& State)
{  

    UE_LOG(LogTemp, Warning, TEXT("Update Info"));
    
    if (State.OwnWitch == CooperationGameGameMode->ActivePlayers[0])
    {
        PlayerInfos[CooperationGameGameMode->ActivePlayers[0]].CurrentHP = State.CurrentHP;
        PlayerInfos[CooperationGameGameMode->ActivePlayers[0]].CurrentEXP = State.CurrentEXP;
        PlayerInfos[CooperationGameGameMode->ActivePlayers[0]].CurrentMana = State.CurrentMana;
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
        PlayerInfos[CooperationGameGameMode->ActivePlayers[1]].CurrentHP = State.CurrentHP;
        PlayerInfos[CooperationGameGameMode->ActivePlayers[1]].CurrentEXP = State.CurrentEXP;
        PlayerInfos[CooperationGameGameMode->ActivePlayers[1]].CurrentMana = State.CurrentMana;
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
    //위젯 접근해서 갱신
    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            // 여기서 UISubsystem 사용 가능!
            Cast<UCooperationWidget>(UISubsystem->CurrentActiveWidget)->InitPlayerUI(&Player1StateData, &Player2StateData);
        }
    }
}

void ACooperationGameState::UpdatePlayerUIInfo()
{
    //위젯 접근해서 갱신
    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            // 여기서 UISubsystem 사용 가능!
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

    if (player == PlayerControllerSet[0]) //<- 이게 맞나?
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
    //UI 끄라고 명령

    bIsPlayerBuffSelect = 0;
}

void ACooperationGameState::AddExperienceToPlayer(AActor* Player, int32 Amount)
{
    if (Player)
    {
        // PlayerInfos에서 플레이어 정보 찾기
        FPlayerData* PlayerData = PlayerInfos.Find(Player);

        if (PlayerData)
        {
            PlayerData->CurrentEXP += Amount;
            CheckLevelUp(Player);
            UpdatePlayerUIInfo(); // UI 갱신
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

//맵의 컨트롤러 등록해두기
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


    for (AActor* Player : CooperationGameGameMode->ActivePlayers)
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
    if (CooperationGameGameMode->ActivePlayers.Num() > 0)
    {
        MeanPlayerLocation = SumPlayerLocation / CooperationGameGameMode->ActivePlayers.Num();
    }

    if (HasAuthority()) // 서버인지 확인
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

    for (AActor* Player : CooperationGameGameMode->ActivePlayers)
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

    int32 NumOfActors = CooperationGameGameMode->ActivePlayers.Num() + CooperationGameGameMode->ActiveEnemies.Num();

    if (CooperationGameGameMode->ActivePlayers.Num() + CooperationGameGameMode->ActiveEnemies.Num() > 0)
    {
        MeanActorLocation = (SumPlayerLocation + SumEnemyLocation) / NumOfActors;
    }


    if (HasAuthority())
    {
        CameraDistance = maxY - minY + 2 * (maxZ - minZ);
        CameraLocation = MeanActorLocation;
        CameraRotation = FRotator::ZeroRotator;

    }// 서버인지 확인

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


//원래보스 납치패턴용 보스 존재

//








////////////////////////////멀티 처리

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
            // 여기서 UISubsystem 사용 가능!
            Cast<UCooperationWidget>(UISubsystem->CurrentActiveWidget)->UpdateBossTimer(Timer);
        }
    }
}

void ACooperationGameState::OnRep_UpdateTimer()
{
    UE_LOG(LogTemp, Warning, TEXT("Timer: %f"), Timer);
    UpdateTimer();
}



void ACooperationGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACooperationGameState, CameraLocation);
    DOREPLIFETIME(ACooperationGameState, CameraRotation);
    DOREPLIFETIME(ACooperationGameState, CameraDistance);
    DOREPLIFETIME(ACooperationGameState, Timer); 
    DOREPLIFETIME(ACooperationGameState, bIsPlayerCanMove); 
    DOREPLIFETIME(ACooperationGameState, CurrentStageIndex);
    DOREPLIFETIME(ACooperationGameState, bIsPlayerBuffSelect);
    DOREPLIFETIME(ACooperationGameState, SelectBuffPlayer);
    DOREPLIFETIME(ACooperationGameState, Player1DataChanged);
    DOREPLIFETIME(ACooperationGameState, Player2DataChanged);
    DOREPLIFETIME(ACooperationGameState, Player1StateData);
    DOREPLIFETIME(ACooperationGameState, Player2StateData);
    DOREPLIFETIME(ACooperationGameState, PlayerDataChanged);

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
            // 여기서 UISubsystem 사용 가능!
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
    Multicast_OnDeathPlayer(Player, DeathLocation);

}

void ACooperationGameState::OnDeathMonster(AActor* Monster, const FVector& DeathLocation) 
{
    Multicast_OnDeathMonster(Monster, DeathLocation);
}



void ACooperationGameState::Multicast_ApplyDamage_Implementation(AActor* Attacker, float Damage, const FVector& HitLocation)
{
    // 모든 클라이언트에서 실행됨
    if (IsValid(Attacker))
    {
        // 예: 피격 위치에 이펙트 생성
        // UGameplayStatics::SpawnEmitterAtLocation(...);
    }

    // 실제 데미지 처리도 필요하다면 여기서 실행 (또는 서버에서만 할 수도 있음)
}

void ACooperationGameState::Multicast_TakeDamage_Implementation(AActor* Victim, float Damage, const FVector& HitLocation)
{
    if (IsValid(Victim))
    {
        // 예: 피해 반응 애니메이션 재생
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

        // 예: 사망 애니메이션, 효과 등
        // Player->PlayDeathAnimation(); 같은 함수 호출
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


//델리게이트로 브로드 캐스트
//게임모드에서 Adddynamic으로 받아서 쓰기./