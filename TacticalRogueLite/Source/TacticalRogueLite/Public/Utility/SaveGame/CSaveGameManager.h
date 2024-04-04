#pragma once

#include "CoreMinimal.h"
#include "Utility/CRandomComponent.h"
#include "CSaveGameManager.generated.h"

UCLASS(BlueprintType)
class UCSaveGameManager : public UObject
{
	GENERATED_BODY()
	
public:
	// Singleton Accessor
	UFUNCTION(BlueprintPure, Category = "SaveGameManager", meta = (DisplayName = "Get Save Game Manager"))
	static UCSaveGameManager* Get();

	// Register Savable Object
	void RegisterSavable(ICSavable* Savable)
	{
		Savables.AddUnique(Savable);
	}
	
	// UnRegister Savable Object
	void UnRegisterSavable(ICSavable* Savable)
	{
		Savables.RemoveSingle(Savable);
	}
	
	// Save Game
	UFUNCTION(BlueprintCallable)
	void SaveGame();

	// Load Game
	UFUNCTION(BlueprintCallable)
	void LoadGame();

	// Get Save Game Instance
	UFUNCTION(BlueprintCallable)
	bool TryGetSaveGame(UCSaveGame* inSaveGame);


private:
	// Trigger Save Event on all Savable Objects
	void TriggerSaveEvent();

	// Trigger Load Event on all Savable Objects
	void TriggerLoadEvent();

	// Registered Savable Objects
	TArray<ICSavable*> Savables;
	
	// Singleton Instance
	static UCSaveGameManager* Instance;

	// Save Game Instance
	UPROPERTY()
	UCSaveGame* SaveGameInstance;
	
	// Save Slot Name
	FString SaveSlot = "SaveSlot";

	// User Index
	int32 UserIndex = 0;
	
};
