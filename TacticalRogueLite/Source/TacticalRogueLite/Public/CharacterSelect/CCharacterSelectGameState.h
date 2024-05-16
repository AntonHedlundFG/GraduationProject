
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CCharacterSelectGameState.generated.h"

class UCCharacterSelectMenuWidget;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReadyToStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateUI);

class UCSetControllingPlayerWidget;
class UCCharacterSelectorWidget;
/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API ACCharacterSelectGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ACCharacterSelectGameState();

	UPROPERTY(BlueprintAssignable, Category = "UI|Update")
	FReadyToStart OnReadyToStart;
	UPROPERTY(BlueprintAssignable, Replicated, Category = "UI|Update")
	FUpdateUI OnUpdateUI;

	UPROPERTY(BlueprintReadWrite, Replicated)
	TArray<FString> PlayerNames;
	
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_ControllingPlayerIndex)
	TArray<int> ControllingPlayerIndex;
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_CharacterIndexes)
	TArray<int> CharacterIndexes;
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_SpriteIndexes)
	TArray<int> SpriteIndexes;
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_LockedInfo)
	TArray<bool> LockedInfo;
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_ReadyInfo)
	TArray<bool> ReadyInfo;
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_PlayerCount)
	int PlayerCount;

	UFUNCTION(BlueprintCallable)
	void UpdateControllingPlayers(TArray<int> inArray);
	UFUNCTION(BlueprintCallable)
	void UpdateCharacters(TArray<int> inArray);
	UFUNCTION(BlueprintCallable)
	void UpdateLocks(TArray<bool> inArray);
	UFUNCTION(BlueprintCallable)
	void UpdateReadyStatus(TArray<bool> inArray);
	UFUNCTION(BlueprintCallable)
	void UpdatePlayerCount(int inCount);
	UFUNCTION(BlueprintCallable)
	void UpdateSprites(TArray<int> inArray);
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_SetupUI();
	UFUNCTION()
	void SetPlayerCountAndLocks(int inPlayerCount);



protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TObjectPtr<UCCharacterSelectMenuWidget> MenuWidget;

	UFUNCTION()
	void CheckReady();

	UFUNCTION(BlueprintCallable)
	void OnRep_ControllingPlayerIndex();
	UFUNCTION(BlueprintCallable)
	void OnRep_CharacterIndexes();
	UFUNCTION(BlueprintCallable)
	void OnRep_LockedInfo();
	UFUNCTION(BlueprintCallable)
	void OnRep_ReadyInfo();
	UFUNCTION(BlueprintCallable)
	void OnRep_SpriteIndexes();
	UFUNCTION(BlueprintCallable)
	void OnRep_PlayerCount() const { OnUpdateUI.Broadcast(); }
};
