

#include "CharacterSelect/CCharacterSelectGameState.h"

#include "CharacterSelect/CCharacterSelectGameMode.h"
#include "CharacterSelect/CCharacterSelectMenuWidget.h"
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

void ACCharacterSelectGameState::UpdateControllingPlayers(TArray<int> inArray)
{
	if (inArray.Num() == ControllingPlayerIndex.Num())
	{
		ControllingPlayerIndex = inArray;
	}

	OnRep_ControllingPlayerIndex();
}

void ACCharacterSelectGameState::UpdateCharacters(TArray<int> inArray)
{
	if (inArray.Num() == ControllingPlayerIndex.Num())
	{
		CharacterIndexes = inArray;
	}
	
	OnRep_CharacterIndexes();
}

void ACCharacterSelectGameState::UpdateLocks(TArray<bool> inArray)
{
	if (inArray.Num() == LockedInfo.Num())
	{
		LockedInfo = inArray;
	}
	OnRep_LockedInfo();
}

void ACCharacterSelectGameState::UpdateReadyStatus(TArray<bool> inArray)
{
	if (inArray.Num() == ReadyInfo.Num())
	{
		ReadyInfo = inArray;
	}
	
	OnRep_ReadyInfo();
	CheckReady();
}

void ACCharacterSelectGameState::UpdatePlayerCount(int inCount)
{
	PlayerCount = inCount;
	OnRep_PlayerCount();
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
	UpdatePlayerCount(inPlayerCount);
	UpdateControllingPlayers(ControllingPlayerIndex);
	UpdateLocks(LockedInfo);
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

void ACCharacterSelectGameState::OnRep_ControllingPlayerIndex()
{
	if (MenuWidget)
		MenuWidget->UpdateControllingPlayers(ControllingPlayerIndex);
}

void ACCharacterSelectGameState::OnRep_CharacterIndexes()
{
	if (MenuWidget)
		MenuWidget->UpdateCharacters(CharacterIndexes);
}

void ACCharacterSelectGameState::OnRep_LockedInfo()
{
	if (MenuWidget)
		MenuWidget->UpdateLocks(LockedInfo);
}

void ACCharacterSelectGameState::OnRep_ReadyInfo()
{
	if (MenuWidget)	
		MenuWidget->UpdateReadyStatus(ReadyInfo);
}


