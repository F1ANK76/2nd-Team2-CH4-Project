// AudioDataSettings.h

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "AudioDataSettings.generated.h"

/**
 * 
 */
UCLASS(Config = AudioDataSettings, DefaultConfig, meta = (DisplayName = "AudioData Settings"))
class ORIGINALSINPRJ_API UAudioDataSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Config, Category = "Sounds")
	TMap<uint8, TSoftObjectPtr<USoundBase>> LevelSounds;

	UPROPERTY(EditAnywhere, Config, Category = "Sounds")
	TMap<uint8, TSoftObjectPtr<USoundBase>> MonsterSounds;
	
	UPROPERTY(EditAnywhere, Config, Category = "Sounds")
	TMap<uint8, TSoftObjectPtr<USoundBase>> CharacterSounds;

	UPROPERTY(EditAnywhere, Config, Category = "Sounds")
	TMap<uint8, TSoftObjectPtr<USoundBase>> BossSounds;
};
