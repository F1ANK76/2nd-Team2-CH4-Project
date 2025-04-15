// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/CooperationGameState.h"
#include "Kismet/GameplayStatics.h" 
#include "GameMode/CooperationGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Player/Controller/WitchController.h"
#include "../GameInstance/DataSubsystem.h"

ACooperationGameState::ACooperationGameState()
{
    PrimaryActorTick.bCanEverTick = true;
    SetActorTickEnabled(false); // 일단 꺼두기

    bIsStage3Started = false;
    Timer = 0.0f;
    bReplicates = true;

    bPlayer1SelectedBuff = false;
    bPlayer2SelectedBuff = false;



    CameraLocation = FVector::ZeroVector;


    CameraRotation = FRotator::ZeroRotator;


    CameraDistance = 0;

}

void ACooperationGameState::BeginPlay()
{
    Super::BeginPlay();
    
    // 게임 스테이트 클래스에서 게임 모드에 접근
    AGameModeBase* GameModeBase = UGameplayStatics::GetGameMode(this);
    CooperationGameGameMode = Cast<ACooperationGameMode>(GameModeBase);

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
    }
}



//ShowLevelWidget


//보스전 타이머 켜기
void ACooperationGameState::TurnOnTimer()
{
    bIsStage3Started = true;
}

void ACooperationGameState::InitPlayerInfo()
{
    PlayerInfos.Empty(); // 기존 정보 삭제

    // 각 플레이어 정보 초기화
    /*
    플레이어의 정보를 받아오고 맵에 저장하기.
    */

}
void ACooperationGameState::UpdatePlayerInfo()
{
    // 각 플레이어 정보 업뎃
/*

*/
}
void ACooperationGameState::InitPlayerUIInfo()
{
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PlayerController = Cast<APlayerController>(*It);
        if (PlayerController)
        {
            //UISubSystem이나 gameInstance에게 UI 띄우라고 명령
            //Player에게 UI정보 바꾸라고 명령.
            //여기에 있는 PlayerInfo 활용해서...
        }
    }
}

void ACooperationGameState::UpdatePlayerUIInfo()
{
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PlayerController = Cast<APlayerController>(*It);
        if (PlayerController)
        {
            //Player에게 UI정보 바꾸라고 명령.
            //여기에 있는 PlayerInfo 활용해서...
        }
    }
}

//플레이어에게 버프 선택 UI 열도록 시키기

void ACooperationGameState::RequestPlayerToOpenBuffUI()
{
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PlayerController = Cast<APlayerController>(*It);
        if (PlayerController)
        {
            //Player에게 BuffSelect U열라고 명령
            //버프 종류는 여기서 결정.
            //버프 종류가 기록되어있는 목록을 받아서, 겹치지 않게 세개를 받아 선택지에 올리기
            /*
            TArray<FBuffType> Buff = {}; //  { 1번, 2번, ... , n번 };
            TArray<FBuffType> SelectedBuff;
            if (Buff.Num() > 0)
            {
                int RandomIndex = FMath::RandRange(0, Buff.Num() - 1);
            }

            for (int i = 0; i < 3; i++)
            {
                int32 Index = FMath::RandRange(0, Buff.Num() - 1);
                SelectedBuff.Add(Buff[Index]);
                Buff.RemoveAt(Index);
            }
            */
            //PlayerController?->UISubsystem->OpenWidget(EWidgetType::BuffSelectWidget);
            //PlayerController?->BuffSelectWidget->InitializeBuffs(SelectedBuff);
        }
    }
}

void ACooperationGameState::RequestPlayerToOpenResultUI()
{
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PlayerController = Cast<APlayerController>(*It);
        if (PlayerController)
        {
            //Player에게 Result UI 열라고 명령
            //UI에 들어갈 결과는 게임모드에서 받아오던가, 여기에 있는 데이터로 표시...

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

        bPlayer1SelectedBuff = true;
    }

    if (player == PlayerControllerSet[1])
    {
        Player2Stage1SelectedBuff = Bufftype;
        bPlayer2SelectedBuff = true;
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
            Player1Stage1SelectedBuff;
            Player2Stage1SelectedBuff;
            */
        }
        else if (CooperationGameGameMode->StageIndex == 3)
        {
            /*
            PlayerInfo += 
            Player1Stage2SelectedBuff;
            Player2Stage2SelectedBuff;
            */
        }
    }
    

    bPlayer1SelectedBuff = false;
    bPlayer2SelectedBuff = false;
}

void ACooperationGameState::AddExperienceToPlayer(APlayerController* Player, int32 Amount)
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

void ACooperationGameState::CheckLevelUp(APlayerController* Player)
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

void ACooperationGameState::OnRep_UpdatePlayerDataUI()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0); // 보통 로컬플레이어
    if (APlayerController* MyPC = Cast<APlayerController>(PC))
    {
        // 로컬 플레이어인지 확인
        if (MyPC->IsLocalPlayerController())
        {
            // 로컬 플레이어일 경우 로그 출력
            //UE_LOG(LogTemp, Log, TEXT("This is the local player controller."));

            // 로컬 플레이어의 UI를 업데이트
            //MyPC->UpdateHealthUI(Health);
        }
        else
        {
            // 로컬 플레이어가 아닐 경우 로그 출력
            UE_LOG(LogTemp, Log, TEXT("This is NOT the local player controller."));
        }
    }

}

void ACooperationGameState::OnRep_UpdateTimer()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0); // 보통 로컬플레이어
    if (AWitchController* MyPC = Cast<AWitchController>(PC))
    {
        // 로컬 플레이어인지 확인
        if (MyPC->IsLocalPlayerController())
        {
            // 로컬 플레이어일 경우 로그 출력
            UE_LOG(LogTemp, Warning, TEXT("MyPC: %s | Role: %s | LocalController: %s"),
                *MyPC->GetName(),
                *UEnum::GetValueAsString(MyPC->GetLocalRole()),
                MyPC->IsLocalPlayerController() ? TEXT("Yes") : TEXT("No"));
            DisableInput(PC);
            UE_LOG(LogTemp, Warning, TEXT("Possessed Pawn: %s"), *GetNameSafe(MyPC->GetPawn()));

            // 로컬 플레이어의 UI를 업데이트
            //MyPC->UpdateHealthUI(Health);
        }
        else
        {
            // 로컬 플레이어가 아닐 경우 로그 출력
            UE_LOG(LogTemp, Log, TEXT("This is NOT the local player controller."));
        }
    }
}



void ACooperationGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);


    DOREPLIFETIME(ACooperationGameState, bPlayer1SelectedBuff);
    DOREPLIFETIME(ACooperationGameState, bPlayer2SelectedBuff);
    DOREPLIFETIME(ACooperationGameState, CameraLocation);
    DOREPLIFETIME(ACooperationGameState, CameraRotation);
    DOREPLIFETIME(ACooperationGameState, CameraDistance);
    DOREPLIFETIME(ACooperationGameState, PlayerDatas);
    DOREPLIFETIME(ACooperationGameState, Timer);

}



void ACooperationGameState::SetPlayerUnReady_Implementation()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0); // 보통 로컬플레이어

    if (APlayerController* MyPC = Cast<APlayerController>(PC))
    {
        // 로컬 플레이어인지 확인
        if (MyPC->IsLocalPlayerController())
        {
            // 로컬 플레이어일 경우 로그 출력
            UE_LOG(LogTemp, Log, TEXT("This is the local player controller."));
            MyPC->DisableInput(MyPC);
            // 로컬 플레이어의 UI를 업데이트
            //MyPC->UpdateHealthUI(Health);
        }
        else
        {
            // 로컬 플레이어가 아닐 경우 로그 출력
            UE_LOG(LogTemp, Log, TEXT("This is NOT the local player controller."));
        }
    }
}