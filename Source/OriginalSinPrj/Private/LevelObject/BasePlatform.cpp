#include "LevelObject/BasePlatform.h"


#include "Net/UnrealNetwork.h"
#include "Components/BoxComponent.h"

ABasePlatform::ABasePlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(RootComponent);
	Box->InitBoxExtent(FVector(50.0f, 50.0f, 10.0f));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
}

void ABasePlatform::BeginPlay()
{
	Super::BeginPlay();

}

void ABasePlatform::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABasePlatform, Scene);
	DOREPLIFETIME(ABasePlatform, Box);
	DOREPLIFETIME(ABasePlatform, StaticMesh);
}

void ABasePlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator DeltaRotation(0.0f, 10.0f * DeltaTime, 0.0f);
	AddActorLocalRotation(DeltaRotation);
}

