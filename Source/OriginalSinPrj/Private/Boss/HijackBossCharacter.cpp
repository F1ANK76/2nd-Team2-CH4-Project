// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/HijackBossCharacter.h"

// Sets default values
AHijackBossCharacter::AHijackBossCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHijackBossCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHijackBossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHijackBossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

