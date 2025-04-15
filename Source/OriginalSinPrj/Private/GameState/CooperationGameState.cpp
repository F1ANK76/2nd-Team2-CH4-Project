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
    SetActorTickEnabled(false); // �ϴ� ���α�

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
    
    // ���� ������Ʈ Ŭ�������� ���� ��忡 ����
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


//������ Ÿ�̸� �ѱ�
void ACooperationGameState::TurnOnTimer()
{
    bIsStage3Started = true;
}

void ACooperationGameState::InitPlayerInfo()
{
    PlayerInfos.Empty(); // ���� ���� ����

    // �� �÷��̾� ���� �ʱ�ȭ
    /*
    �÷��̾��� ������ �޾ƿ��� �ʿ� �����ϱ�.
    */

}
void ACooperationGameState::UpdatePlayerInfo()
{
    // �� �÷��̾� ���� ����
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
            //UISubSystem�̳� gameInstance���� UI ����� ���
            //Player���� UI���� �ٲٶ�� ���.
            //���⿡ �ִ� PlayerInfo Ȱ���ؼ�...
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
            //Player���� UI���� �ٲٶ�� ���.
            //���⿡ �ִ� PlayerInfo Ȱ���ؼ�...
        }
    }
}

//�÷��̾�� ���� ���� UI ������ ��Ű��

void ACooperationGameState::RequestPlayerToOpenBuffUI()
{
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PlayerController = Cast<APlayerController>(*It);
        if (PlayerController)
        {
            //Player���� BuffSelect U����� ���
            //���� ������ ���⼭ ����.
            //���� ������ ��ϵǾ��ִ� ����� �޾Ƽ�, ��ġ�� �ʰ� ������ �޾� �������� �ø���
            /*
            TArray<FBuffType> Buff = {}; //  { 1��, 2��, ... , n�� };
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
            //Player���� Result UI ����� ���
            //UI�� �� ����� ���Ӹ�忡�� �޾ƿ�����, ���⿡ �ִ� �����ͷ� ǥ��...

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

void ACooperationGameState::OnRep_UpdatePlayerDataUI()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0); // ���� �����÷��̾�
    if (APlayerController* MyPC = Cast<APlayerController>(PC))
    {
        // ���� �÷��̾����� Ȯ��
        if (MyPC->IsLocalPlayerController())
        {
            // ���� �÷��̾��� ��� �α� ���
            //UE_LOG(LogTemp, Log, TEXT("This is the local player controller."));

            // ���� �÷��̾��� UI�� ������Ʈ
            //MyPC->UpdateHealthUI(Health);
        }
        else
        {
            // ���� �÷��̾ �ƴ� ��� �α� ���
            UE_LOG(LogTemp, Log, TEXT("This is NOT the local player controller."));
        }
    }

}

void ACooperationGameState::OnRep_UpdateTimer()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0); // ���� �����÷��̾�
    if (AWitchController* MyPC = Cast<AWitchController>(PC))
    {
        // ���� �÷��̾����� Ȯ��
        if (MyPC->IsLocalPlayerController())
        {
            // ���� �÷��̾��� ��� �α� ���
            UE_LOG(LogTemp, Warning, TEXT("MyPC: %s | Role: %s | LocalController: %s"),
                *MyPC->GetName(),
                *UEnum::GetValueAsString(MyPC->GetLocalRole()),
                MyPC->IsLocalPlayerController() ? TEXT("Yes") : TEXT("No"));
            DisableInput(PC);
            UE_LOG(LogTemp, Warning, TEXT("Possessed Pawn: %s"), *GetNameSafe(MyPC->GetPawn()));

            // ���� �÷��̾��� UI�� ������Ʈ
            //MyPC->UpdateHealthUI(Health);
        }
        else
        {
            // ���� �÷��̾ �ƴ� ��� �α� ���
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
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0); // ���� �����÷��̾�

    if (APlayerController* MyPC = Cast<APlayerController>(PC))
    {
        // ���� �÷��̾����� Ȯ��
        if (MyPC->IsLocalPlayerController())
        {
            // ���� �÷��̾��� ��� �α� ���
            UE_LOG(LogTemp, Log, TEXT("This is the local player controller."));
            MyPC->DisableInput(MyPC);
            // ���� �÷��̾��� UI�� ������Ʈ
            //MyPC->UpdateHealthUI(Health);
        }
        else
        {
            // ���� �÷��̾ �ƴ� ��� �α� ���
            UE_LOG(LogTemp, Log, TEXT("This is NOT the local player controller."));
        }
    }
}