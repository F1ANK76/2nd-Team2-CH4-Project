#include "DataSubsystem.h"
#include "AllDataSettings.h"
#include "Struct/CharacterDataStruct.h"
void UDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	AllDataSettings = GetDefault<UAllDataSettings>();

    LoadDataTables();
}

void UDataSubsystem::LoadDataTables()
{
    if (AllDataSettings)
    {
        if (!AllDataSettings->BuffDataTable.IsNull())
        {
            BuffDataTable = AllDataSettings->BuffDataTable.LoadSynchronous();
        }

        if (!AllDataSettings->CharacterDataTable.IsNull())
        {
            CharacterDataTable = AllDataSettings->CharacterDataTable.LoadSynchronous();
        }
    }
}
