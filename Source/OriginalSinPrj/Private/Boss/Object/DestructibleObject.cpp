// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Object/DestructibleObject.h"


ADestructibleObject::ADestructibleObject()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ADestructibleObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADestructibleObject::OnPooledObjectSpawn_Implementation()
{
	IBossPoolableActorInterface::OnPooledObjectSpawn_Implementation();
}

void ADestructibleObject::OnPooledObjectReset_Implementation()
{
	IBossPoolableActorInterface::OnPooledObjectReset_Implementation();
}