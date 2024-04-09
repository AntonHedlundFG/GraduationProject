

#include "CharacterSelect/CCharacterSelectGameState.h"

#include "CharacterSelect/CCharacterSelectGameMode.h"
#include "Net/UnrealNetwork.h"


void ACCharacterSelectGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACCharacterSelectGameState, PlayerCount);
	DOREPLIFETIME(ACCharacterSelectGameState, CharacterIndexes);
	DOREPLIFETIME(ACCharacterSelectGameState, ControllingPlayerIndex);
	DOREPLIFETIME(ACCharacterSelectGameState, LockedInfo);
}

ACCharacterSelectGameState::ACCharacterSelectGameState()
{
	if (const ACCharacterSelectGameMode* Mode = Cast<ACCharacterSelectGameMode>(GetDefaultGameMode()))
	{
		PlayerCount = Mode->GetPlayerCount();
	}
	ControllingPlayerIndex = TArray{1,1,1,1};
	CharacterIndexes = TArray{0,0,0,0};
	LockedInfo = TArray{false, false, false, false};
}

void ACCharacterSelectGameState::OnRep_UpdateControllingPlayers(TArray<int> inArray)
{
	if (inArray.Num() == ControllingPlayerIndex.Num())
	{
		ControllingPlayerIndex = inArray;
		OnRep_UpdateUI();
	}
}

void ACCharacterSelectGameState::OnRep_UpdateCharacters(TArray<int> inArray)
{
	if (inArray.Num() == ControllingPlayerIndex.Num())
	{
		CharacterIndexes = inArray;
		OnRep_UpdateUI();
	}
}

void ACCharacterSelectGameState::OnRep_UpdateLocks(TArray<bool> inArray)
{
	if (inArray.Num() == LockedInfo.Num())
	{
		LockedInfo = inArray;
		OnRep_UpdateUI();
	}
}

