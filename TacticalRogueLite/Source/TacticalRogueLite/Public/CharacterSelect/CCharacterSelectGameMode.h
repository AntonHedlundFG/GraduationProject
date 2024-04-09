
#pragma once

#include "CoreMinimal.h"
#include "TacticalRogueLite/OnlineSystem/Public/OnlineGameMode.h"
#include "Utility/SaveGame/CSavable.h"
#include "CCharacterSelectGameMode.generated.h"


struct FCNamesAndItemsList;
class ACCharacterSelectGameState;
class UCStartCharacterData;
/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API ACCharacterSelectGameMode : public AOnlineGameMode, public ICSavable
{
	GENERATED_BODY()

public:
	ACCharacterSelectGameMode();
	
	UPROPERTY(EditAnywhere, Category = "Player Count")
	int8 DefaultPlayerCount = 2;

	UFUNCTION(BlueprintPure)
	int GetPlayerCount() const { return PlayerCount; }

	UFUNCTION(BlueprintPure)
	ACCharacterSelectGameState* GetStateRef() { return StateRef;}
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	ACCharacterSelectGameState* StateRef;
	UPROPERTY()
	int8 PlayerCount;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCStartCharacterData> StartCharacters;
	UPROPERTY()
	TArray<FCNamesAndItemsList> SpawnUnitData;

	UFUNCTION()
	void CreateSaveGameAndStart();

	virtual void OnSave() override;
	virtual void OnLoad() override;
	
};
