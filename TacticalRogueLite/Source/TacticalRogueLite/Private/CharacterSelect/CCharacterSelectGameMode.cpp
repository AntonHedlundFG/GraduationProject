

#include "CharacterSelect/CCharacterSelectGameMode.h"

#include "CLevelURLAsset.h"
#include "CharacterSelect/CCharacterSelectGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/Logging/CLogManager.h"

ACCharacterSelectGameMode::ACCharacterSelectGameMode()
{
	PlayerCount = UGameplayStatics::GetIntOption(OptionsString, NUMBER_OF_PLAYERS, DefaultPlayerCount);
}

void ACCharacterSelectGameMode::BeginPlay()
{
	Super::BeginPlay();

	StateRef = GetGameState<ACCharacterSelectGameState>();
	if(StateRef)
	{
		StateRef->PlayerCount = PlayerCount;
		StateRef->OnReadyToStart.AddDynamic(this, &ACCharacterSelectGameMode::SaveGameAndStart);
	}
}

void ACCharacterSelectGameMode::SaveGameAndStart()
{
	LOG_INFO("Ready To Start");
}
