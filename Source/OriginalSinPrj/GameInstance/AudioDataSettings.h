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
	TSoftObjectPtr<UDataTable> LevelSounds;

	UPROPERTY(EditAnywhere, Config, Category = "Sounds")
	TSoftObjectPtr<UDataTable> UISfxSounds;

	UPROPERTY(EditAnywhere, Config, Category = "Sounds")
	TSoftObjectPtr<UDataTable> MonsterSounds;

	UPROPERTY(EditAnywhere, Config, Category = "Sounds")
	TSoftObjectPtr<UDataTable> CharacterSounds;

	UPROPERTY(EditAnywhere, Config, Category = "Sounds")
	TSoftObjectPtr<UDataTable> BossSounds;
};
