#include "SpawnManager.h"
#include "Player/Controller/WitchController.h"

ASpawnManager::ASpawnManager()
{
 	PrimaryActorTick.bCanEverTick = true;

}

void ASpawnManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

APawn* ASpawnManager::SpawnPlayer(AWitchController* Controller, FVector SpawnLocation)
{
	APawn* NewCharacter;
	if (Controller)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = Controller;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;


		NewCharacter = GetWorld()->SpawnActor<APawn>(
			PlayerClass, 
			SpawnLocation, 
			FRotator::ZeroRotator, 
			SpawnParams
		);

		if (NewCharacter)
		{
			Controller->Possess(NewCharacter);
			NewCharacter->PossessedBy(Controller);
		}

		return NewCharacter;
	}

	return nullptr;
}

void ASpawnManager::SpawnMonster(AController* Controller, FVector SpawnLocation)
{
	if (Controller)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = Controller;

		APawn* NewMonster = GetWorld()->SpawnActor<APawn>(
			MonsterClass,
			SpawnLocation,
			FRotator::ZeroRotator,
			SpawnParams
		);

		if (NewMonster)
		{
			Controller->Possess(NewMonster);
		}
	}
}

