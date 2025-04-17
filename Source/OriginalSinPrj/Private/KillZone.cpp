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

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(RootComponent);
	Box->InitBoxExtent(FVector(10000.0f, 10000.0f, 100.0f));
	Box->OnComponentBeginOverlap.AddDynamic(this, &AKillZone::OnOverlapBegin);
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
	UE_LOG(LogTemp, Warning, TEXT("Overlap "));
	if (ABaseWitch* Witch = Cast<ABaseWitch>(OtherActor))
	{
		Server_TriggerOverlapWitchEvent(Witch);
	}
	else if (AMonsterCharacter* Monster = Cast<AMonsterCharacter>(OtherActor))
	{
		Server_TriggerOverlapMonsterEvent(Monster);
	}
	// ĳ���� ���������� �� ĳ���� �״� �̺�Ʈ �ۼ� �ʿ�
}

void AKillZone::Server_TriggerOverlapMonsterEvent_Implementation(AMonsterCharacter* Monster)
{
	UE_LOG(LogTemp, Warning, TEXT("Server_TriggerOverlapMonsterEvent_Implementation Overlap Event Begin"));

	if (ACooperationGameMode* CoopMode = Cast<ACooperationGameMode>(GetWorld()->GetAuthGameMode()))
	{
		CoopMode->OnDeathPlayer(Monster, Monster->GetActorLocation());
	}
}

void AKillZone::Server_TriggerOverlapWitchEvent_Implementation(ABaseWitch* WitchActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Server_TriggerOverlapWitchEvent_Implementation Overlap Event Begin"));

	WitchActor->DecreaseLifePoint();
	WitchActor->OnOverlapedDeathZone();
}
