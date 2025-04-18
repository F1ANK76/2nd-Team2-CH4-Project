#include "GameMode/MultiBattleGameMode.h"
#include "GameState/MultiBattleGameState.h"
#include "LevelObjectManager.h"
#include "SpawnManager.h"
#include "Player/Controller/WitchController.h"
#include "Player/BaseWitch.h"
#include "OriginalSinPrj/GameInstance/OriginalSinPrjGameInstance.h"
#include "OriginalSinPrj/GameInstance/Struct/BuffDataStruct.h"

#include "Kismet/GameplayStatics.h"
#include "OriginalSinPrj/GameInstance/UISubsystem.h"

AMultiBattleGameMode::AMultiBattleGameMode()
	: CurrentActorArrayIndex(0)
	, LevelObjectManager(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultPawnClass = ABaseWitch::StaticClass();
	bUseSeamlessTravel = true; // Seamless Travel
	GameStateClass = AMultiBattleGameState::StaticClass();
	PlayerControllerClass = AWitchController::StaticClass();
}

void AMultiBattleGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// 엔진이 알아서 폰 스폰 & Possess & 복제까지 모두 처리
	FTransform SpawnTransform(FRotator::ZeroRotator, PlayerSpawnLocations[SpawnLocationIndex]);
	RestartPlayerAtTransform(NewPlayer, SpawnTransform);
	SpawnLocationIndex++;

	APawn* Pawn = NewPlayer->GetPawn();
	if (ABaseWitch* Witch = Cast<ABaseWitch>(Pawn))
	{
		SpawnedCharacters.Add(Witch);
		ActivePlayers.Add(Witch);
		AlivePlayers.Add(Witch);
		Witch->SetHpMode(false);
	}
	
	Cast<AWitchController>(NewPlayer)->ResponseShowLevelWidget();

	if (SpawnLocationIndex == 2)
	{
		StartGame();
	}
}

void AMultiBattleGameMode::HandleSeamlessTravelPlayer(AController*& C)
{
	Super::HandleSeamlessTravelPlayer(C);

	// C가 내 컨트롤러 타입인지 체크
	AWitchController* PC = Cast<AWitchController>(C);
	if (!PC) return;

	// 1) 스폰 트랜스폼 계산(이미 스폰된 Pawn이 남아 있다면 스폰 로직이 아니라 Possess 로직만 필요할 수도 있고요)
	FTransform SpawnTransform(FRotator::ZeroRotator, PlayerSpawnLocations[SpawnLocationIndex]);
	RestartPlayerAtTransform(PC, SpawnTransform);
	SpawnLocationIndex++;

	// 2) Pawn 얻어서 리스트에 추가
	if (ABaseWitch* Witch = Cast<ABaseWitch>(PC->GetPawn()))
	{
		SpawnedCharacters.Add(Witch);
		ActivePlayers.Add(Witch);
		AlivePlayers.Add(Witch);
		Witch->SetHpMode(false);
	}

	// 3) UI 갱신
	PC->ResponseShowLevelWidget();

	// 4) 두 명 모두 처리된 뒤에 게임 시작
	if (SpawnLocationIndex == 2)
	{
		StartGame();
	}
}

APawn* AMultiBattleGameMode::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	int32 CharacterTypeIndex = 0;
	if (UOriginalSinPrjGameInstance* GI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
	{
		CharacterTypeIndex = int32(GI->GetSelectedCharacterType());
	}
	
	UClass* ChosenClass = WitchClasses[CharacterTypeIndex];
	FTransform SpawnTransform = StartSpot ? StartSpot->GetActorTransform() : FTransform::Identity;
	FActorSpawnParameters Params;
	Params.Owner = NewPlayer;
	
	return GetWorld()->SpawnActor<APawn>(ChosenClass, SpawnTransform, Params);
}

void AMultiBattleGameMode::BeginPlay()
{
	Super::BeginPlay();

	MultiBattleGameState = GetWorld()->GetGameState<AMultiBattleGameState>();
	
	SpawnCamera();
	InitPlayerUI();
	
	LevelObjectManager = GetWorld()->SpawnActor<ALevelObjectManager>(LevelObjectManagerClass);
	SpawnManager = GetWorld()->SpawnActor<ASpawnManager>(SpawnManagerClass);

	InitializeTempObjects();
	LevelObjectManager->SpawnDeathZone();
}

void AMultiBattleGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MultiBattleGameState->SetCameraTransform();
}

void AMultiBattleGameMode::StartGame()
{	
	SpawnedCharacters[0]->OnChangedState.AddDynamic(this, &AMultiBattleGameMode::OnCharacterStateReceived);
	SpawnedCharacters[1]->OnChangedState.AddDynamic(this, &AMultiBattleGameMode::OnCharacterStateReceived);

	AttachPlayerToCamera(SpawnedCharacters[0], SpawnedBaseCamera[0]);
	AttachPlayerToCamera(SpawnedCharacters[1], SpawnedBaseCamera[0]);
	
	MultiBattleGameState->InitPlayerInfo();
	MultiBattleGameState->PlayerDataChanged++;
	MultiBattleGameState->TurnOnBattleWidget();

	UE_LOG(LogTemp, Warning, TEXT("PlayerInfos.Num() : %d"), MultiBattleGameState->PlayerInfos.Num());
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
		if (IsValid(MyGI))
		{
			MyGI->ResponseShowWidget();
		}
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

void AMultiBattleGameMode::RequestTurnOnBuffSelectUI(AWitchController* Controller)
{
	Controller->Client_CreateBuffWidget();
}

void AMultiBattleGameMode::RequestTurnOffBuffSelectUI()
{
	MultiBattleGameState->SelectBuffPlayer++;
	MultiBattleGameState->CloseBuffSelectUI();
}

void AMultiBattleGameMode::ApplyBuffToBothPlayer()
{
	//���� �ΰ� ������� ���� �����϶�� ��û.
	if (IsValid(MultiBattleGameState))
	{
		MultiBattleGameState->ApplyBuffStat();
	}
	RequestTurnOffBuffSelectUI();
}

void AMultiBattleGameMode::HandlePlayerKilled(AActor* DeadPlayer, AActor* Killer)
{

	UE_LOG(LogTemp, Warning, TEXT("HandlePlayerKilled Event Begin"));
	UE_LOG(LogTemp, Warning, TEXT("ActivePlayers Size: %d"), ActivePlayers.Num());
	UE_LOG(LogTemp, Warning, TEXT("AlivePlayers Size: %d"), AlivePlayers.Num());
	if (CurrentPlayerCount > 1)
	{
		if (!IsValid(DeadPlayer))
		{
			UE_LOG(LogTemp, Warning, TEXT("DeadPlayer Is invalid"));
		}

		if (DeadPlayer == AlivePlayers[0])
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerWitch1 Die"));
			AlivePlayers.Remove(DeadPlayer);
		}
		else if (DeadPlayer == AlivePlayers[1])
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerWitch2 Die"));
			AlivePlayers.Remove(DeadPlayer);
		}
	}
	else if (CurrentPlayerCount > 0)
	{
		if (DeadPlayer == AlivePlayers[0])
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerWitch1 Die"));
			AlivePlayers.Remove(DeadPlayer);
		}
	}
	else
	{

	}
	CurrentPlayerCount--;



	//ActivePlayers.Remove(DeadPlayer); // 알아서 내부에서 찾고 제거함 너무 위험.
	//DeadPlayer->Destroy();   // 너무 위험
	//test Code
	if (CurrentPlayerCount <= 0)
	{
		bIsClear = false;
		SetPlayerUnReady();

		FTimerHandle DefeatHandle;
		GetWorldTimerManager().SetTimer(DefeatHandle, FTimerDelegate::CreateLambda([this]()
		{
			EndGame();
		}), 5.0f, false);
	}
}

void AMultiBattleGameMode::SetPlayerUnReady()
{
	MultiBattleGameState->SetPlayerMove(false);
	UWorld* WorldContext = GetWorld();

	for (FConstPlayerControllerIterator It = WorldContext->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC && PC->GetPawn())
		{
			FString ControllerName = PC->GetName();
			FString PawnName = PC->GetPawn()->GetName();

			UE_LOG(LogTemp, Warning, TEXT("Disabling input for PlayerController: %s, Pawn: %s"), *ControllerName, *PawnName);

			PC->GetPawn()->DisableInput(PC);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerController or Pawn is null (PC: %s)"), PC ? *PC->GetName() : TEXT("nullptr"));
		}
	}
}

void AMultiBattleGameMode::SetPlayerReady()
{
	MultiBattleGameState->SetPlayerMove(true);
    
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

void AMultiBattleGameMode::PlayerFallDie(AActor* DeadPlayer, AActor* Killer)
{
	ABaseWitch* Witch = Cast<ABaseWitch>(DeadPlayer);

	
	FPlayerData* Data = nullptr;
	if (ActivePlayers[0] == DeadPlayer)
	{
		*Data = MultiBattleGameState->PlayerInfos[0];
	}
	if (ActivePlayers[1] == DeadPlayer)
	{
		*Data = MultiBattleGameState->PlayerInfos[0];
	}

	Data->LifePoint--;
	
	//test Code
	if (IsValid(Witch))
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerWitch Die"));
		if (Data->LifePoint < 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerWitch kill"));
			HandlePlayerKilled(DeadPlayer, Killer);
			Witch->ResetCharacterState();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerWitch respawn"));
			Respawn(Witch);
			Witch->ResetCharacterState();
		}
	}
}

void AMultiBattleGameMode::EndGame()
{

	//�ϴ� �÷��̾� �Է��� ���߱�.
	SetPlayerUnReady();

	//RequestOpenResultUI();
	if (bIsClear == true) // GameClear
	{

	}
	else
	{

	}
	UE_LOG(LogTemp, Warning, TEXT("Game End "));
}

void AMultiBattleGameMode::Respawn(AActor* DeadPlayer)
{
	UE_LOG(LogTemp, Warning, TEXT("PlayerWitch respawn"));

	if (!IsValid(DeadPlayer))
	{
		return;
	}
	
	ABaseWitch* Witch = Cast<ABaseWitch>(DeadPlayer);
	if (Witch)
	{
		if (AWitchController* WitchController = Cast<AWitchController>(Witch->GetController()))
		{
			RequestTurnOnBuffSelectUI(WitchController);
		}
	}

	FVector SpawnLocation = LevelObjectManager->GetTopPlatformLocation();
	DeadPlayer->SetActorLocation(SpawnLocation); 
}

void AMultiBattleGameMode::PostSeamlessTravel()
{
	UE_LOG(LogTemp, Warning, TEXT("PostSeamlessTravel Called"));
	Super::PostSeamlessTravel();  // 기본 SeamlessTravel 처리

	if (UOriginalSinPrjGameInstance* GI = Cast<UOriginalSinPrjGameInstance>(GetGameInstance()))
	{
		if (UUISubsystem* UISub = GI->GetSubsystem<UUISubsystem>())
		{
			if (UISub->CurrentActiveWidget)
			{
				UISub->CurrentActiveWidget->RemoveFromParent();
				UISub->CurrentActiveWidget = nullptr;
			}
		}
	}

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (AWitchController* PC = Cast<AWitchController>(*It))
		{
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

void AMultiBattleGameMode::OnDeathPlayer(AActor* Player)
{
	ABaseWitch* Witch = Cast<ABaseWitch>(Player);

	if (IsValid(Player))
	{
		if (ActivePlayers[0] == Player)
		{
			MultiBattleGameState->PlayerInfos[0].LifePoint--;
			UE_LOG(LogTemp, Warning, TEXT("LifePoint : %d"), MultiBattleGameState->PlayerInfos[0].LifePoint);
			
			if (MultiBattleGameState->PlayerInfos[0].LifePoint < 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("PlayerWitch kill"));
				HandlePlayerKilled(Player, Player);
				Witch->ResetCharacterState();
			}
			else
			{
				if (IsValid(Witch))
				{
					UE_LOG(LogTemp, Warning, TEXT("PlayerWitch respawn"));
					Respawn(Witch);
					Witch->ResetCharacterState();
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("액터가 캐릭터로 인식되지 않음"));
				}
			}
		}
		if (ActivePlayers[1] == Player)
		{
			MultiBattleGameState->PlayerInfos[1].LifePoint--;
			UE_LOG(LogTemp, Warning, TEXT("LifePoint : %d"), MultiBattleGameState->PlayerInfos[1].LifePoint);
			
			if (MultiBattleGameState->PlayerInfos[1].LifePoint < 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("PlayerWitch kill"));
				HandlePlayerKilled(Player, Player);
				Witch->ResetCharacterState();
			}
			else
			{
				if (IsValid(Witch))
				{
					UE_LOG(LogTemp, Warning, TEXT("PlayerWitch respawn"));
					Respawn(Witch);
					Witch->ResetCharacterState();
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("액터가 캐릭터로 인식되지 않음"));
				}
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("PlayerWitch Die"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("액터가 nullptr"));
	}
}

void AMultiBattleGameMode::OnDeathPlayer(ACharacter* Player, const FVector& DeathLocation)
{
	
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
