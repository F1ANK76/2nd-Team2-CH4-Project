#include "DeathZone.h"
#include "LevelObject/BasePlatform.h"
#include "Player/BaseWitch.h"
#include "GameMode/MultiBattleGameMode.h"
#include "OriginalSinPrj/Public/SingleGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Components/BoxComponent.h"

ADeathZone::ADeathZone()
	: MoveSpeed(100.0f)
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	bAlwaysRelevant = true;

	OnActorBeginOverlap.AddDynamic(this, &ADeathZone::OnOverlapBegin);

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(RootComponent);
	Box->InitBoxExtent(FVector(1000.0f, 1000.0f, 100.0f));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
}

void ADeathZone::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADeathZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CurrentLocation = GetActorLocation();
	
	CurrentLocation.Z += MoveSpeed * DeltaTime;

	SetActorLocation(CurrentLocation);
}

void ADeathZone::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADeathZone, Scene);
	DOREPLIFETIME(ADeathZone, Box);
	DOREPLIFETIME(ADeathZone, StaticMesh);
}

void ADeathZone::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && (OtherActor != this))
	{
		if (ABasePlatform* Platform = Cast<ABasePlatform>(OtherActor))
		{
			TriggerOverlapPlatformEvent();
		}
	}

	if (OtherActor && (OtherActor != this))
	{
		if (ABaseWitch* Witch = Cast<ABaseWitch>(OtherActor))
		{
			Server_TriggerOverlapWitchEvent(Witch);
		}
	}

	// 캐릭터 오버랩됐을 때 캐릭터 죽는 이벤트 작성 필요
}

void ADeathZone::TriggerOverlapPlatformEvent()
{
	UE_LOG(LogTemp, Warning, TEXT("플랫폼 오버랩 이벤트 발생"));
	OnMyOverlapEvent.Broadcast();
}

void ADeathZone::Server_TriggerOverlapWitchEvent_Implementation(ABaseWitch* WitchActor)
{
	if (HasAuthority())
	{
		if (WitchActor->IsPlayerControlled())
		{
			if (AMultiBattleGameMode* MultiBattleGameMode = Cast<AMultiBattleGameMode>(GetWorld()->GetAuthGameMode()))
			{
				MultiBattleGameMode->OnDeathPlayer(WitchActor, WitchActor->GetActorLocation());
				UE_LOG(LogTemp, Warning, TEXT("MultiBattleGameMode OnDeathPlayer"));
			}
			else if (ASingleGameMode* SingleGameMode = Cast<ASingleGameMode>(GetWorld()->GetAuthGameMode()))
			{
				SingleGameMode->OnDeathPlayer(WitchActor, WitchActor->GetActorLocation());
			}
		}
		else
		{
			if (AMultiBattleGameMode* MultiBattleGameMode = Cast<AMultiBattleGameMode>(GetWorld()->GetAuthGameMode()))
			{
				MultiBattleGameMode->OnDeathMonster(WitchActor, WitchActor->GetActorLocation());
				UE_LOG(LogTemp, Warning, TEXT("MultiBattleGameMode OnDeathMonster"));
			}
			else if (ASingleGameMode* SingleGameMode = Cast<ASingleGameMode>(GetWorld()->GetAuthGameMode()))
			{
				SingleGameMode->OnDeathMonster(WitchActor, WitchActor->GetActorLocation());
			}
		}
	}
}
