#include "TestPlatform.h"

#include "Net/UnrealNetwork.h"
#include "Components/BoxComponent.h"

ATestPlatform::ATestPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(RootComponent);
	Box->InitBoxExtent(FVector(50.0f, 50.0f, 50.0f));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
}

void ATestPlatform::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATestPlatform::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATestPlatform, Scene);
	DOREPLIFETIME(ATestPlatform, Box);
	DOREPLIFETIME(ATestPlatform, StaticMesh);
}

void ATestPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator DeltaRotation(0.0f, 10.0f * DeltaTime, 0.0f);
	AddActorLocalRotation(DeltaRotation);
}

