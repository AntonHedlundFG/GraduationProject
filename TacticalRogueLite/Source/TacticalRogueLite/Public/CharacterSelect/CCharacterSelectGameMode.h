
#pragma once

#include "CoreMinimal.h"
#include "TacticalRogueLite/OnlineSystem/Public/OnlineGameMode.h"
#include "Utility/SaveGame/CSavable.h"
#include "CCharacterSelectGameMode.generated.h"


class UCLevelURLAsset;
struct FCNamesAndItemsList;
class ACCharacterSelectGameState;
class UCStartCharacterData;
class UCRandomComponent;
/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API ACCharacterSelectGameMode : public AOnlineGameMode, public ICSavable
{
	GENERATED_BODY()

public:
	ACCharacterSelectGameMode();

	UPROPERTY(EditAnywhere, Category = "Level Loading")
	TObjectPtr<UCLevelURLAsset> LevelURLAsset;
	
	UPROPERTY(EditAnywhere, Category = "Player Count")
	int8 DefaultPlayerCount = 2;

	UFUNCTION(BlueprintPure)
	int GetPlayerCount() const { return PlayerCount; }

	UFUNCTION(BlueprintPure)
	ACCharacterSelectGameState* GetStateRef() { return StateRef;}
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category = "Random")
	TObjectPtr<UCRandomComponent> Random;
	UPROPERTY()
	ACCharacterSelectGameState* StateRef;
	UPROPERTY()
	int8 PlayerCount;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCStartCharacterData> StartCharacters;
	UPROPERTY()
	TArray<FCNamesAndItemsList> SpawnUnitData;
	UPROPERTY()
	TArray<int> ControllingPlayers;

	UFUNCTION()
	void CreateSaveGameAndStart();

	virtual void OnSave() override;
	virtual void OnLoad() override;

	FString GetNameOfPlayerIndex(int32 Index);
	
};
