#include "KillZone.h"
#include "LevelObject/BasePlatform.h"
#include "Player/BaseWitch.h"
#include "GameMode/CooperationGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Components/BoxComponent.h"
#include "AI/MonsterCharacter.h" 

AKillZone::AKillZone()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	bAlwaysRelevant = true;

	Box->OnComponentBeginOverlap.AddDynamic(this, &AKillZone::OnOverlapBegin);

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(RootComponent);
	Box->InitBoxExtent(FVector(1000.0f, 1000.0f, 100.0f));
}

void AKillZone::BeginPlay()
{
	Super::BeginPlay();

}


void AKillZone::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKillZone, Scene);
	DOREPLIFETIME(AKillZone, Box);
}


void AKillZone::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (ABaseWitch* Witch = Cast<ABaseWitch>(OtherActor))
	{
		Server_TriggerOverlapWitchEvent(Witch);
	}
	else if (AMonsterCharacter* Monster = Cast<AMonsterCharacter>(OtherActor))
	{
		Server_TriggerOverlapMonsterEvent(Monster);
	}
	// 캐릭터 오버랩됐을 때 캐릭터 죽는 이벤트 작성 필요
}

void AKillZone::TriggerOverlapPlatformEvent()
{
	OverlapEvent.Broadcast();
}

void AKillZone::Server_TriggerOverlapMonsterEvent_Implementation(AMonsterCharacter* Monster)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap Event Begin"));

	if (ACooperationGameMode* CoopMode = Cast<ACooperationGameMode>(GetWorld()->GetAuthGameMode()))
	{
		CoopMode->OnDeathPlayer(Monster, Monster->GetActorLocation());
	}
}

void AKillZone::Server_TriggerOverlapWitchEvent_Implementation(ABaseWitch* WitchActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap Event Begin"));

	if (ACooperationGameMode* CoopMode = Cast<ACooperationGameMode>(GetWorld()->GetAuthGameMode()))
	{
		CoopMode->OnDeathPlayer(WitchActor, WitchActor->GetActorLocation());
	}
}
