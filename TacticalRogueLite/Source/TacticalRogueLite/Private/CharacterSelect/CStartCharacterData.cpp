

#include "CharacterSelect/CStartCharacterData.h"

#include "GridContent/UnitDataAssets/CUnitSpawnDetails.h"
#include "Utility/Logging/CLogManager.h"

FString UCStartCharacterData::GetCharacterType(int index)
{
	if (index > StartCharacterList.Num())
	{
		LOG_ERROR("Invalid index for start character list");
		return "";
	}
	return StartCharacterList[index]->ClassName;
}

UPaperSprite* UCStartCharacterData::GetCharacterSprite(int ClassIndex, int SpriteIndex)
{
	int ClampedClassIndex = FMath::Clamp(ClassIndex,0,StartCharacterList.Num() -1);
	int ClampedSpriteIndex = FMath::Clamp(SpriteIndex,0, StartCharacterList[ClampedClassIndex]->ClassSprites.Num()-1);
	return StartCharacterList[ClampedClassIndex]->ClassSprites[ClampedSpriteIndex];
}

int UCStartCharacterData::GetSpriteListLength(int ClassIndex)
{
	int ClampedClassIndex = FMath::Clamp(ClassIndex,0,StartCharacterList.Num() -1);
	return StartCharacterList[ClampedClassIndex]->ClassSprites.Num();
}

TArray<UCItemData*> UCStartCharacterData::GetCharacterItems(int index)
{
	if (index > StartCharacterList.Num())
	{
		LOG_ERROR("Invalid index for start character list");
		return TArray<UCItemData*>();
	}
	return StartCharacterList[index]->Items;
}

FGameplayTag UCStartCharacterData::GetCharacterTag(int i)
{
	int ClampedClassIndex = FMath::Clamp(i,0,StartCharacterList.Num() -1);
	return StartCharacterList[i]->ClassTag;
}
