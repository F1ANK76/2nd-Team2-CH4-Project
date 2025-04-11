#include "DataSubsystem.h"
#include "AllDataSettings.h"

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
    }
}
