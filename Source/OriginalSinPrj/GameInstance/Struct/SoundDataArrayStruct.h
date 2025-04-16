// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoundDataArrayStruct.generated.h"

struct FBossAudioDataStruct;
struct FCharacterAudioDataStruct;
struct FLevelAudioDataStruct;
struct FMonsterAudioDataStruct;
struct FUISfxAudioDataStruct;

USTRUCT(BlueprintType)
struct ORIGINALSINPRJ_API FSoundDataArrayStruct
{
	GENERATED_BODY()
	
public:
	TArray<FBossAudioDataStruct*> BossAudioArray;
	TArray<FCharacterAudioDataStruct*> CharacterAudioArray;
	TArray<FLevelAudioDataStruct*> BgmAudioArray;
	TArray<FMonsterAudioDataStruct*> MonsterAudioArray;
	TArray<FUISfxAudioDataStruct*> UIAudioArray;
};
