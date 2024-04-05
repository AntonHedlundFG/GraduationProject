

#include "CharacterSelect/CCharacterSelectorWidget.h"

#include "CharacterSelect/CStartCharacterData.h"

void UCCharacterSelectorWidget::IncreaseIndex()
{
	CharacterIndex++;
	if (CharacterIndex >= StartCharacters->StartCharacterList.Num())
		CharacterIndex = 0;
}

void UCCharacterSelectorWidget::DecreaseIndex()
{
	CharacterIndex--;
	if (CharacterIndex < 0 )
		CharacterIndex = StartCharacters->StartCharacterList.Num() - 1;
}
