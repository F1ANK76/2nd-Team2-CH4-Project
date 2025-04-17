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

	MultiBattleGameState = GetWorld()->GetGameState<AMultiBattleGameState>();
	
	SpawnCamera();
	InitPlayerUI();
	
	LevelObjectManager = GetWorld()->SpawnActor<ALevelObjectManager>(LevelObjectManagerClass);
	SpawnManager = GetWorld()->SpawnActor<ASpawnManager>(SpawnManagerClass);

	GetWorldTimerManager().SetTimer(
		DelayTimer,
		this,
		&AMultiBattleGameMode::IsStart,
		0.1f,
		false
	);
}

void AMultiBattleGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (AWitchController* WitchController = Cast<AWitchController>(NewPlayer))
	{
		APawn* SpawnedPlayer = SpawnManager->SpawnPlayer(WitchController, FVector(0, 100.0f, 0));

		if (ABaseWitch* BaseWitch = Cast<ABaseWitch>(SpawnedPlayer))
		{
			BaseWitch->OnChangedState.AddDynamic(this, &AMultiBattleGameMode::OnCharacterStateReceived);
			AttachPlayerToCamera(BaseWitch, SpawnedBaseCamera[0]);

			SpawnedCharacters.Add(BaseWitch);
			ActivePlayers.Add(BaseWitch);
			CurrentPlayerCount++;

			BaseWitch->SetHpMode(false);
		}
		MultiBattleGameState->RegisterInitialController(WitchController);
		WitchController->ResponseShowLevelWidget();
	}
}

void AMultiBattleGameMode::IsStart()
{
	if (SpawnedCharacters.Num() >= 2)
	{
		GetWorldTimerManager().ClearTimer(DelayTimer);
		StartGame();
	}
}

void AMultiBattleGameMode::StartGame()
{
	MultiBattleGameState->InitPlayerInfo();
	MultiBattleGameState->PlayerDataChanged++;
	MultiBattleGameState->TurnOnBattleWidget();	

	InitializeTempObjects();
	LevelObjectManager->SpawnDeathZone();
}

void AMultiBattleGameMode::SpawnPlayers()
{
	// ĳ���� Ÿ�� ���� (��: �⺻ ĳ���� Ŭ����)
	TArray<UClass*> CharacterClasses = { ABaseWitch::StaticClass(), ABaseWitch::StaticClass() };

	for (int32 i = 0; i < CharacterClasses.Num(); ++i)
	{
		UClass* SpawnClass = CharacterClasses[i];

		if (HasAuthority() && IsValid(SpawnClass))
		{
			// �÷��̾� ���� ���� ã��
			AActor* StartActor = FindPlayerStart(nullptr);  // Ư�� �÷��̾��� ���� ���� ã��
			if (!StartActor) continue;

			FVector StartPos = StartActor->GetActorLocation();

			FActorSpawnParameters SpawnParam;
			SpawnParam.Owner = nullptr;  // �⺻������ ����
			SpawnParam.Instigator = nullptr;  // �⺻������ ����

			// ĳ���� ����
			ABaseWitch* SpawnedCharacter = GetWorld()->SpawnActor<ABaseWitch>(SpawnClass, PlayerSpawnLocations[i], FRotator::ZeroRotator, SpawnParam);

			if (SpawnedCharacter)
			{
				SpawnedCharacters.Add(SpawnedCharacter);
				ActivePlayers.Add(SpawnedCharacter);
				CurrentPlayerCount++;

				SpawnedCharacter->SetHpMode(false);

				UE_LOG(LogTemp, Warning, TEXT("Spawned Pawn: %s"), *GetNameSafe(SpawnedCharacter));
			}
		}
	}

	// SpawnedCharacters �迭�� ������ ĳ���͵��� Ȯ���� �� �ֽ��ϴ�.
	for (ABaseWitch* Character : SpawnedCharacters)
	{
		UE_LOG(LogTemp, Warning, TEXT("Managed Character: %s"), *GetNameSafe(Character));
	}

	AlivePlayers.Empty(); // ���� ������ �����
	for (AActor* Player : ActivePlayers)
	{
		if (Player) // null üũ �� ���� ���� �� ����
		{
			AlivePlayers.Add(Player);
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

	for (int i = 0; i < 2; i++)
	{
		FRotator SpawnRotation = FRotator::ZeroRotator;

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		ABaseCamera* SpawnedCamera = World->SpawnActor<ABaseCamera>(BaseCamera, FVector::ZeroVector, SpawnRotation, SpawnParams);
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

void AMultiBattleGameMode::PossessCharacter(APlayerController* PC, APawn* PawnToPossess)
{
	if (!PC || !PawnToPossess) return;

	PawnToPossess->SetOwner(PC);
	PC->Possess(PawnToPossess);

	UE_LOG(LogTemp, Warning, TEXT("Possessed Pawn: %s by Controller: %s"), *GetNameSafe(PawnToPossess), *GetNameSafe(PC));
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