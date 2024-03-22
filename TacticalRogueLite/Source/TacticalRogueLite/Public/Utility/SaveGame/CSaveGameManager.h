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
	void RegisterSavable(UCRandomComponent* RandomComponent)
	{
		Savables.AddUnique(RandomComponent);
	}
	
	// Save Game
	UFUNCTION(BlueprintCallable)
	void SaveGame();

	// Load Game
	UFUNCTION(BlueprintCallable)
	class UCSaveGame* LoadGame();

	// Get Save Game Instance
	UFUNCTION(BlueprintCallable)
	UCSaveGame* GetSaveGameInstance();

	// UnRegister Savable Object
	void UnRegisterSavable(UCRandomComponent* RandomComponent) { Savables.RemoveSingle(RandomComponent); }

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
