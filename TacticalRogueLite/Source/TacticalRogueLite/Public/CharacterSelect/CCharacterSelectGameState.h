
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
	UFUNCTION(BlueprintCallable)
	void OnRep_UpdateUI() const { OnUpdateUI.Broadcast(); }
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_SetupUI();
	UFUNCTION()
	void SetPlayerCountAndLocks(int inPlayerCount);


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TObjectPtr<UCCharacterSelectMenuWidget> MenuWidget;

	UFUNCTION()
	void CheckReady();
	
};
