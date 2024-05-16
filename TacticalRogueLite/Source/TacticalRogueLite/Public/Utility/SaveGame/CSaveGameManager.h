#pragma once

#include "CoreMinimal.h"
#include "Utility/CRandomComponent.h"
#include "CSaveGameManager.generated.h"

class UCSaveGame_Settings;
class USaveGame;
class UCSaveGame;

UENUM(BlueprintType)
enum class ESaveGameType : uint8
{
    NONE            UMETA(Hidden),
    ESGT_Game       UMETA(DisplayName = "Game"),
    ESGT_Settings   UMETA(DisplayName = "Settings"),
    MAX             UMETA(Hidden)
};

UCLASS(BlueprintType)
class UCSaveGameManager : public UObject
{
    GENERATED_BODY()

public:
    // Singleton Accessor
    UFUNCTION(BlueprintPure, Category = "SaveGameManager", meta = (DisplayName = "Get Save Game Manager"))
    static UCSaveGameManager* Get();

    // Register Savable Object
    void RegisterSavable(ESaveGameType SaveGameType, ICSavable* Savable);

    // UnRegister Savable Object
    void UnRegisterSavable(ESaveGameType SaveGameType, ICSavable* Savable);

    // Create New Save Game Instance, Doesn't Save
    UFUNCTION(BlueprintCallable)
    USaveGame* CreateNewSave(ESaveGameType SaveGameType);

    // Save Game
    UFUNCTION(BlueprintCallable)
    void SaveGame(ESaveGameType SaveGameType);

    // Load Game
    UFUNCTION(BlueprintCallable)
    void LoadGame(ESaveGameType SaveGameType);

    // Get Save Game Instance
    UFUNCTION(BlueprintCallable)
    bool TryGetSaveGame(ESaveGameType SaveGameType, USaveGame*& inSaveGame);

private:
    // Trigger Save Event on all Savable Objects of a specific type
    void TriggerSaveEvent(ESaveGameType SaveGameType);

    // Trigger Load Event on all Savable Objects of a specific type
    void TriggerLoadEvent(ESaveGameType SaveGameType);

    // Registered Savable Objects
    TMap<ESaveGameType, TArray<ICSavable*>> Savables;

    // Singleton Instance
    static UCSaveGameManager* Instance;

    // Save Game Instances
    UPROPERTY()
    TMap<ESaveGameType, USaveGame*> SaveGameInstances;

    // Save Slot Names
    UPROPERTY()
    TMap<ESaveGameType, FString> SaveSlots;

    // User Index
    int32 UserIndex = 0;
};
