

#include "CharacterSelect/CCharacterSelectGameState.h"

#include "CharacterSelect/CCharacterSelectGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Utility/Logging/CLogManager.h"


void ACCharacterSelectGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACCharacterSelectGameState, PlayerCount);
	DOREPLIFETIME(ACCharacterSelectGameState, CharacterIndexes);
	DOREPLIFETIME(ACCharacterSelectGameState, ControllingPlayerIndex);
	DOREPLIFETIME(ACCharacterSelectGameState, LockedInfo);
	DOREPLIFETIME(ACCharacterSelectGameState, ReadyInfo);
	DOREPLIFETIME(ACCharacterSelectGameState, OnUpdateUI);
}

ACCharacterSelectGameState::ACCharacterSelectGameState()
{
	ControllingPlayerIndex = TArray{1,1,1,1};
	CharacterIndexes = TArray{0,0,0,0};
	LockedInfo = TArray{false, false, false, false};
	ReadyInfo = TArray{false, false, false, false};
}

void ACCharacterSelectGameState::OnRep_UpdateControllingPlayers(TArray<int> inArray)
{
	if (inArray.Num() == ControllingPlayerIndex.Num())
	{
		ControllingPlayerIndex = inArray;
	}
}

void ACCharacterSelectGameState::OnRep_UpdateCharacters(TArray<int> inArray)
{
	if (inArray.Num() == ControllingPlayerIndex.Num())
	{
		CharacterIndexes = inArray;
	}
}

void ACCharacterSelectGameState::OnRep_UpdateLocks(TArray<bool> inArray)
{
	if (inArray.Num() == LockedInfo.Num())
	{
		LockedInfo = inArray;
	}
}

void ACCharacterSelectGameState::OnRep_UpdateReadyStatus(TArray<bool> inArray)
{
	if (inArray.Num() == ReadyInfo.Num())
	{
		ReadyInfo = inArray;
		OnRep_UpdateUI();
	}
	
	CheckReady();
}

void ACCharacterSelectGameState::OnRep_UpdatePlayerCount(int inCount)
{
	PlayerCount = inCount;
}

void ACCharacterSelectGameState::SetPlayerCountAndLocks(int inPlayerCount)
{
	for (int i = 0; i < ControllingPlayerIndex.Num(); i ++)
	{
		if (i < inPlayerCount)
		{
			ControllingPlayerIndex[i] = i + 1;
			LockedInfo[i] = true;
		}
		else
		{
			ControllingPlayerIndex[i] = 1;
			LockedInfo[i] = false;
		}
	}
	OnRep_UpdatePlayerCount(inPlayerCount);
	OnRep_UpdateControllingPlayers(ControllingPlayerIndex);
	OnRep_UpdateLocks(LockedInfo);
}

void ACCharacterSelectGameState::CheckReady()
{
	for (const bool PlayerReady : ReadyInfo)
	{
		if (PlayerReady == false)
			return;
	}
	
	OnReadyToStart.Broadcast();
}

