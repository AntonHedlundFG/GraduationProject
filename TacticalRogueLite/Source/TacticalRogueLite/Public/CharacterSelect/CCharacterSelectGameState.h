
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CCharacterSelectGameState.generated.h"

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

	UPROPERTY(BlueprintAssignable, Category = "Update UI")
	FReadyToStart OnReadyToStart;
	UPROPERTY(BlueprintAssignable, Category = "Update UI")
	FUpdateUI OnUpdateUI;

	UPROPERTY(BlueprintReadWrite, Replicated)
	TArray<int> ControllingPlayerIndex;
	UPROPERTY(BlueprintReadWrite, Replicated)
	TArray<int> CharacterIndexes;
	UPROPERTY(BlueprintReadWrite, Replicated)
	TArray<bool> LockedInfo;
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_UpdateUI)
	TArray<bool> ReadyInfo;
	UPROPERTY(BlueprintReadWrite, Replicated)
	int PlayerCount;

	UFUNCTION(BlueprintCallable)
	void OnRep_UpdateControllingPlayers(TArray<int> inArray);
	UFUNCTION(BlueprintCallable)
	void OnRep_UpdateCharacters(TArray<int> inArray);
	UFUNCTION(BlueprintCallable)
	void OnRep_UpdateLocks(TArray<bool> inArray);
	UFUNCTION(BlueprintCallable)
	void OnRep_UpdateReadyStatus(TArray<bool> inArray);
	UFUNCTION(BlueprintCallable)
	void OnRep_UpdatePlayerCount(int inCount);
	UFUNCTION()
	void OnRep_UpdateUI() { OnUpdateUI.Broadcast(); }
	UFUNCTION()
	void SetPlayerCountAndLocks(int inPlayerCount);


protected:
	void CheckReady();
	
};
