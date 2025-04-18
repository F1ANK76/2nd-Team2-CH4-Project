#include "DeathZone.h"
#include "LevelObject/BasePlatform.h"
#include "Player/BaseWitch.h"
#include "GameMode/MultiBattleGameMode.h"
#include "OriginalSinPrj/Public/SingleGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Components/BoxComponent.h"

ADeathZone::ADeathZone()
	: MoveSpeed(15.0f)
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
	// 캐릭터 오버랩됐을 때 캐릭터 죽는 이벤트 작성 필요
}

void ADeathZone::TriggerOverlapPlatformEvent()
{
	UE_LOG(LogTemp, Warning, TEXT("플랫폼 오버랩 이벤트 발생"));
	OnMyOverlapEvent.Broadcast();
}
