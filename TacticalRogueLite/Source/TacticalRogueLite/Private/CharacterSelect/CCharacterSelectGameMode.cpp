

#include "CharacterSelect/CCharacterSelectGameMode.h"

#include "CLevelURLAsset.h"
#include "CharacterSelect/CCharacterSelectGameState.h"
#include "CharacterSelect/CStartCharacterData.h"
#include "GridContent/UnitDataAssets/CUnitSpawnDetails.h"
#include "TacticalRogueLite/OnlineSystem/Public/EpicOnlineSubsystem.h"
#include "Utility/Logging/CLogManager.h"
#include "Utility/SaveGame/CSaveGame.h"
#include "Utility/SaveGame/CSaveGameManager.h"
#include "Utility/CRandomComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TacticalRogueLite/OnlineSystem/Public/OnlinePlayerState.h"

ACCharacterSelectGameMode::ACCharacterSelectGameMode()
{
	Random = CreateDefaultSubobject<UCRandomComponent>(TEXT("Random"));
}



void ACCharacterSelectGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	RegisterToSaveManager();

	UCSaveGameManager::Get()->LoadGame(ESaveGameType::ESGT_Game);
	
	//Create a random seed
	const int Seed = FMath::RandRange(0000,9999);
	Random->InitializeFromStart(Seed);
	
	StateRef = GetGameState<ACCharacterSelectGameState>();
	if(StateRef)
	{
		if (GetNetMode() > NM_Standalone)
			StateRef->PlayerNames = GetAllPlayerNames();
		
		StateRef->SetPlayerCountAndLocks(PlayerCount);
		StateRef->OnReadyToStart.AddDynamic(this, &ACCharacterSelectGameMode::CreateSaveGameAndStart);
		StateRef->BP_SetupUI();
	}

	//Begin-play done !!
}

void ACCharacterSelectGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterFromSaveManager();
}

void ACCharacterSelectGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	if (StateRef)
	{
		const TArray<FString> Names = GetAllPlayerNames();
		StateRef->PlayerNames = Names;
		PlayerCount = Names.Num();
		StateRef->SetPlayerCountAndLocks(PlayerCount);
	}
}

void ACCharacterSelectGameMode::RegisterToSaveManager()
{
	UCSaveGameManager::Get()->RegisterSavable(ESaveGameType::ESGT_Game, this);
}

void ACCharacterSelectGameMode::UnregisterFromSaveManager()
{
	UCSaveGameManager::Get()->UnRegisterSavable(ESaveGameType::ESGT_Game, this);
}

TArray<FString> ACCharacterSelectGameMode::GetAllPlayerNames()
{
	TArray<FString> Names = TArray
	{
		FString("Player 1"),
		FString("Player 2"),
		FString("Player 3"),
		FString("Player 4")
	};

	if (!StateRef)
		return Names;
	
	auto PlayerArray = StateRef->PlayerArray;

	for (auto Player : PlayerArray)
	{
		if (const AOnlinePlayerState* PS = Cast<AOnlinePlayerState>(Player))
		{
			int i = PS->PlayerIndex - 1;
			FString PlayerName = Player->GetPlayerName();
			if (i < Names.Num())
			{
				Names[i] = PlayerName;
			}
		}
	}
		
	return Names;
}

void ACCharacterSelectGameMode::CreateSaveGameAndStart()
{
	if (!StartCharacters)
	{
		LOG_WARNING("No Start Character Data in Game Mode");
		return;
	}
	if (!StateRef)
	{
		LOG_WARNING("No Game State Reference in Game Mode");
		return;
	}
	if (!LevelURLAsset)
	{
		LOG_WARNING("No Level URL Asset in Game Mode");
		return;
	}
	
	TArray<int> Characters = StateRef->CharacterIndexes;
	ControllingPlayers = StateRef->ControllingPlayerIndex;

	SpawnUnitData.Empty();
	int CharacterNumber = 1;
	
	for (int i = 0; i < Characters.Num(); i++)
	{
		FCUnitSpawnDetails Data;
		Data.Sprite = StartCharacters->GetCharacterSprite(Characters[i],StateRef->SpriteIndexes[i]);
		Data.Items = StartCharacters->GetCharacterItems(Characters[i]);
		Data.ClassTag = StartCharacters->GetCharacterTag(Characters[i]);
		FString HeroName = "Hero" + FString::FromInt(CharacterNumber) + ": ";
		FString PlayerName = GetNameOfPlayerIndex(ControllingPlayers[i]);
		if (!PlayerName.IsEmpty() && GetNetMode() > NM_Standalone)
		{
			HeroName += PlayerName;
		}
		else
		{
			HeroName +=StartCharacters->GetCharacterType(Characters[i]);
		}
		Data.Name = HeroName;
		CharacterNumber++;
		SpawnUnitData.Add(Data);
	}

	/*
	*/
	UCSaveGameManager* SaveManager = UCSaveGameManager::Get();
	SaveManager->SaveGame(ESaveGameType::ESGT_Game);
	
	LOG_INFO("Ready To Start");

	FLevelLoadSettings LevelLoadSettings;
	LevelLoadSettings.NumberOfPlayers = GetPlayerCount();

	UnregisterFromSaveManager();
	
	FString URLString = LevelURLAsset->URLOfLevelByName("GameLevel", LevelLoadSettings);
	
	if (UEpicOnlineSubsystem* OnlineSystem = GetGameInstance()->GetSubsystem<UEpicOnlineSubsystem>())
	{
		OnlineSystem->ServerTravel(URLString);
	}
	
}

void ACCharacterSelectGameMode::OnSave()
{
	USaveGame* SaveGameBase = nullptr;
	if (UCSaveGameManager::Get()->TryGetSaveGame(ESaveGameType::ESGT_Game, SaveGameBase))
	{
		if (UCSaveGame* SaveGame = Cast<UCSaveGame>(SaveGameBase))
		{
			//Save Names and Items	
			SaveGame->UnitDetails.Empty();
			for (auto Data : SpawnUnitData)
			{
				SaveGame->UnitDetails.Add(Data);
			}

			//Save Names and Items
			SaveGame->PlayerCount = PlayerCount;
		
			//Save controlling player indexes
			SaveGame->ControllingPlayers.Empty();
			for (auto Data : ControllingPlayers)
			{
				SaveGame->ControllingPlayers.Add(Data);
			}
		}
	}
	else
	{
		LOG_ERROR("Could not find Save Game Instance to save Unit Items and Data in Game Mode");
	}
}

void ACCharacterSelectGameMode::OnLoad()
{
	USaveGame* SaveGameBase = nullptr;
	if (UCSaveGameManager::Get()->TryGetSaveGame(ESaveGameType::ESGT_Game, SaveGameBase))
	{
		if (UCSaveGame* SaveGame = Cast<UCSaveGame>(SaveGameBase))
		{
			PlayerCount = SaveGame->PlayerCount;
		}
	}
	else
	{
		PlayerCount = 1; //Assume solo play when there's no saved game from the lobby.
		LOG_ERROR("Couldn't find Save Game Instance to load Player Count in Character Select Menu");
	}
}

FString ACCharacterSelectGameMode::GetNameOfPlayerIndex(int32 Index)
{
	int NumPlayerStates = UGameplayStatics::GetNumPlayerStates(this);
	for (int i = 0; i < NumPlayerStates; i++)
	{
		AOnlinePlayerState* PS = Cast<AOnlinePlayerState>(UGameplayStatics::GetPlayerState(this, i));
		if (Index != PS->PlayerIndex) continue;

		return PS->GetPlayerName();
	}
	return FString();
}
