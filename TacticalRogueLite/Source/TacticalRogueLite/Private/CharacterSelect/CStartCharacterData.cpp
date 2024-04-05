

#include "CharacterSelect/CStartCharacterData.h"

#include "Utility/Logging/CLogManager.h"

FString UCStartCharacterData::GetCharacterType(int index)
{
	if (index > StartCharacterList.Num())
	{
		LOG_ERROR("Invalid index for start character list");
		return "";
	}
	return StartCharacterList[index].CharacterType;
}

TArray<UCItemData*> UCStartCharacterData::GetCharacterItems(int index)
{
	if (index > StartCharacterList.Num())
	{
		LOG_ERROR("Invalid index for start character list");
		return TArray<UCItemData*>();
	}
	return StartCharacterList[index].ItemList;
}
