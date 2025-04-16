#include "DataSubsystem.h"
#include "AllDataSettings.h"
#include "Struct/CharacterDataStruct.h"
#include "Struct/BuffDataStruct.h"


void UDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	AllDataSettings = GetDefault<UAllDataSettings>();

    //LoadDataTables();
}

const FCharacterDataStruct* UDataSubsystem::GetCharacterDataByType(ECharacterType Type)
{
    if (!CheckValidOfCharacterData())
    {
        return nullptr;
    }

    FCharacterDataStruct* Result = nullptr;

    for (FCharacterDataStruct* CharacterData : CharacterTypeData.CharacterDataArray)
    {
        if (Type == CharacterData->CharacterType)
        {
            Result = CharacterData;
            break;
        }
    }

    return Result;
}

const TArray<FBuffDataStruct*>& UDataSubsystem::GetBuffDataArray()
{
    CheckValidOfBuffData();

    return BuffDataArrayStruct.BuffDataArray;
}

const int32 UDataSubsystem::GetCharacterTypeSize()
{
    if (!CheckValidOfCharacterData())
    {
        return 0;
    }

    return CharacterTypeData.CharacterDataArray.Num();
}

//void UDataSubsystem::LoadDataTables()
//{
//    if (AllDataSettings)
//    {
//        if (!AllDataSettings->BuffDataTable.IsNull())
//        {
//            BuffDataTable = AllDataSettings->BuffDataTable.LoadSynchronous();
//        }
//
//        if (!AllDataSettings->CharacterDataTable.IsNull())
//        {
//            CharacterDataTable = AllDataSettings->CharacterDataTable.LoadSynchronous();
//        }
//    }
//}

bool UDataSubsystem::CheckValidOfCharacterData()
{
    if (CharacterTypeData.CharacterDataArray.IsEmpty())
    {
        if (!IsValid(AllDataSettings))
        {
            return false;
        }

        if (AllDataSettings->CharacterDataTable.IsNull())
        {
            return false;
        }

        const FString DataContext(TEXT("Data ConText"));

        CharacterDataTable = AllDataSettings->CharacterDataTable.LoadSynchronous();
        CharacterDataTable->GetAllRows(DataContext, CharacterTypeData.CharacterDataArray);
    }

    return true;
}

bool UDataSubsystem::CheckValidOfBuffData()
{
    if (BuffDataArrayStruct.BuffDataArray.IsEmpty())
    {
        if (!IsValid(AllDataSettings))
        {
            return false;
        }

        if (AllDataSettings->BuffDataTable.IsNull())
        {
            return false;
        }

        const FString DataContext(TEXT("Data ConText"));

        BuffDataTable = AllDataSettings->BuffDataTable.LoadSynchronous();
        BuffDataTable->GetAllRows(DataContext, BuffDataArrayStruct.BuffDataArray);
    }

    return true;
}
