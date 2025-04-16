#include "GameMode/MultiBattleGameMode.h"
#include "GameState/MultiBattleGameState.h"
#include "LevelObjectManager.h"
#include "SpawnManager.h"
#include "Player/Controller/WitchController.h"
#include "Player/BaseWitch.h"
#include "OriginalSinPrj/GameInstance/OriginalSinPrjGameInstance.h"

#include "Kismet/GameplayStatics.h"

AMultiBattleGameMode::AMultiBattleGameMode()
	: CurrentActorArrayIndex(0)
	, LevelObjectManager(nullptr)
{
	bUseSeamlessTravel = true; // Seamless Travel
	GameStateClass = AMultiBattleGameState::StaticClass();
}

void AMultiBattleGameMode::BeginPlay()
{
	Super::BeginPlay();
	//CreateTestPlatform(FVector::ZeroVector, FRotator::ZeroRotator);

	SpawnCamera();
	InitPlayerUI();
	
	LevelObjectManager = GetWorld()->SpawnActor<ALevelObjectManager>(LevelObjectManagerClass);
	SpawnManager = GetWorld()->SpawnActor<ASpawnManager>(SpawnManagerClass);
	StartDelay();
}

void AMultiBattleGameMode::StartDelay()
{
	FTimerHandle DelayTimer;

	GetWorldTimerManager().SetTimer(
		DelayTimer,
		this,
		&AMultiBattleGameMode::StartGame,
		2.0f,
		false
	);
}

void AMultiBattleGameMode::StartGame()
{
	SpawnedCharacters[0]->OnChangedState.AddDynamic(this, &AMultiBattleGameMode::OnCharacterStateReceived);
	SpawnedCharacters[1]->OnChangedState.AddDynamic(this, &AMultiBattleGameMode::OnCharacterStateReceived);
	
	InitializeTempObjects();
	LevelObjectManager->SpawnDeathZone();

	// 테스트 
	SpawnPlayer();
}

void AMultiBattleGameMode::SpawnPlayer()
{
	float DeltaY = 100.0f;
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC)
		{
			if (AWitchController* WitchController = Cast<AWitchController>(PC))
			{
				UE_LOG(LogTemp, Warning, TEXT("컨트롤러 할당, 플레이어 스폰"));
				SpawnManager->SpawnPlayer(WitchController, FVector(0.0f, 0.0f + DeltaY, 100.0f));
				DeltaY += 100.0f;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("컨트롤러 캐스팅 실패"))
			}
		}
	}
}

void AMultiBattleGameMode::RespawnPlayer(APlayerController* PlayerController)
{
	/*
	플레이어 스테이트에서 플레이어의 정보 가져오기
	리스폰된 액터에 플레이어 정보 저장하기
	*/

	if (PlayerController)
	{
		FVector RespawnLocation;
		if (LevelObjectManager)
		{
			AActor* RespawnedPlatform = LevelObjectManager->GetRespawnPlatform();
			RespawnLocation = RespawnedPlatform->GetActorLocation();
		}
		if (AWitchController* WitchController = Cast<AWitchController>(PlayerController))
		{
			UE_LOG(LogTemp, Warning, TEXT("컨트롤러 할당, 플레이어 스폰"));
			SpawnManager->SpawnPlayer(WitchController, RespawnLocation);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("컨트롤러 캐스팅 실패"))
		}
	}
}

void AMultiBattleGameMode::SpawnCamera()
{
	UWorld* World = GetWorld();
	if (!IsValid(World) || !IsValid(BaseCamera)) return;

	for (const FVector& SpawnLocation : CameraSpawnLocations)
	{
		FRotator SpawnRotation = FRotator::ZeroRotator;

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		ABaseCamera* SpawnedCamera = World->SpawnActor<ABaseCamera>(BaseCamera, SpawnLocation, SpawnRotation, SpawnParams);
		if (SpawnedCamera)
		{
			SpawnedBaseCamera.Add(SpawnedCamera);
		}
	}
}

void AMultiBattleGameMode::AttachPlayerToCamera(ACharacter* Player, ABaseCamera* Camera)
{
	APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
	if (PlayerController == nullptr) return;  // ��Ʈ�ѷ��� ������ ����

	if (PlayerController)
	{
		Camera->ActivateCamera(PlayerController);
	}
}

void AMultiBattleGameMode::InitPlayerUI()
{
	UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance());
	if (MyGI)
	{
		MyGI->ResponseShowWidget();
	}
}

void AMultiBattleGameMode::InitializeTempObjects()
{
	LevelObjectManager->InitializeTempObjects();
}

void AMultiBattleGameMode::SpawnAndDestroyObject()
{
	LevelObjectManager->SpawnAndDestroyObject();
}

void AMultiBattleGameMode::OnCharacterStateReceived(const FCharacterStateBuffer& State)
{
	if (MultiBattleGameState)
	{
		MultiBattleGameState->UpdatePlayerInfo(State);
	}
}

void AMultiBattleGameMode::HandleClientPossession(APlayerController* PC, int index)
{
	UE_LOG(LogTemp, Warning, TEXT("Client Possess start %s"), *GetNameSafe(PC));
	if (PC && SpawnedCharacters.Num() > 0)
	{
		APawn* PawnToPossess = SpawnedCharacters[index];

		if (PawnToPossess && IsValid(PawnToPossess))
		{
			// ���� ��Ʈ�ѷ��� ������ UnPossess ó��
			APlayerController* OldPC = PawnToPossess->GetController<APlayerController>();
			if (OldPC)
			{
				OldPC->UnPossess();  // ���� ��Ʈ�ѷ����� Pawn�� ����
			}
			// ���ο� ��Ʈ�ѷ��� �ش� Pawn�� Possess�ϵ��� ó��
			PC->Possess(PawnToPossess);
			PC->ClientRestart(PawnToPossess); // Ŭ���ʿ� ����� ���� ����


			UE_LOG(LogTemp, Warning, TEXT("Client Possessed Pawn: %s by %s"),
				*GetNameSafe(PawnToPossess), *GetNameSafe(PC));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Pawn to possess is not valid."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController is invalid or no spawned characters."));
	}
	UE_LOG(LogTemp, Warning, TEXT("Client Possess ended %s"), *GetNameSafe(PC));
    
	//Game Stage���� �ʿ��� ����� ���� ���� ������Ʈ�� ��Ʈ�ѷ� ����صα�
	MultiBattleGameState->RegisterInitialController(PC);
}

void AMultiBattleGameMode::PostSeamlessTravel()
{
	UE_LOG(LogTemp, Warning, TEXT("PostSeamlessTravel Called"));
	Super::PostSeamlessTravel();  // �⺻ SeamlessTravel ó��

	//��Ƽ�÷��� ����
	SpawnPlayer();
	int index = 0;
	// Ŭ���̾�Ʈ�� ��Ʈ�ѷ��� ĳ���͸� ��Ī��Ŵ

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AWitchController* PC = Cast<AWitchController>(*It);
		if (PC)
		{
			UE_LOG(LogTemp, Warning, TEXT("Controller Detected: %s"), *GetNameSafe(PC));
			// �������� Ŭ���̾�Ʈ�� Pawn�� Ȯ���ϰ� Possess ó��
			FTimerHandle TimerHandle;
			GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this, PC, index]()
				{
					HandleClientPossession(PC, index);
				}), 1.0f, false);
			index++;

			PC->ResponseShowLevelWidget();
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("PostSeamlessTravel Done"));
}


void AMultiBattleGameMode::ApplyDamage(AActor* Attacker, float Damage, const FVector& HitLocation)
{
	/*
	범위 판정->TakeDamage함수로 HP를 감소시켜야하는 액터를 선정
	*/

	if (AMultiBattleGameState* GS = Cast<AMultiBattleGameState>(UGameplayStatics::GetGameState(this)))
	{
		//MultiBattleGameState->ApplyDamage(); -> 공격을 실행한 액터가 공격 애니메이션을 실행하도록 게임스테이트에서 명령
		//TakeDamage(); -> 공격 범위에 있던 액터의 HP를 감소시키고 피격 애니메이션을 실행하도록 명령
	}
}

void AMultiBattleGameMode::TakeDamage(AActor* Victim, float Damage, const FVector& HitLocation)
{
	if (AMultiBattleGameState* GS= Cast<AMultiBattleGameState>(UGameplayStatics::GetGameState(this)))
	{
		GS->TakeDamage(Victim, Damage, HitLocation);
	}
}

void AMultiBattleGameMode::OnDeathPlayer(ACharacter* Player, const FVector& DeathLocation)
{
	if (AMultiBattleGameState* GS = Cast<AMultiBattleGameState>(UGameplayStatics::GetGameState(this)))
	{
		GS->OnDeathPlayer(Player, DeathLocation);
	}
}

void AMultiBattleGameMode::OnDeathMonster(AActor* Monster, const FVector& DeathLocation)
{

	if (AMultiBattleGameState* GS = Cast<AMultiBattleGameState>(UGameplayStatics::GetGameState(this)))
	{
		GS->OnDeathMonster(Monster, DeathLocation);
	}
}

void AMultiBattleGameMode::FinishMatch()
{
	if (AMultiBattleGameState* GS = Cast<AMultiBattleGameState>(UGameplayStatics::GetGameState(this)))
	{
		GS->FinishMatch();
	}
}

void AMultiBattleGameMode::VictoryMatch()
{
	if (AMultiBattleGameState* GS = Cast<AMultiBattleGameState>(UGameplayStatics::GetGameState(this)))
	{
		GS->VictoryMatch();
	}
}

void AMultiBattleGameMode::DefeatMatch()
{
	if (AMultiBattleGameState* GS = Cast<AMultiBattleGameState>(UGameplayStatics::GetGameState(this)))
	{
		GS->DefeatMatch();
	}
}

void AMultiBattleGameMode::DrawMatch()
{
	if (AMultiBattleGameState* GS = Cast<AMultiBattleGameState>(UGameplayStatics::GetGameState(this)))
	{
		GS->DrawMatch();
	}
}