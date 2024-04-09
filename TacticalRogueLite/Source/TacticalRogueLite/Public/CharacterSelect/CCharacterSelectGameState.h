
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CCharacterSelectGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInitialized);
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
	FInitialized OnInitialized;
	UPROPERTY(BlueprintAssignable, Category = "Update UI")
	FUpdateUI OnUpdateUI;

	UPROPERTY(BlueprintReadWrite, Replicated)
	TArray<int> ControllingPlayerIndex;
	UPROPERTY(BlueprintReadWrite, Replicated)
	TArray<int> CharacterIndexes;
	UPROPERTY(BlueprintReadWrite, Replicated)
	TArray<bool> LockedInfo;
	UPROPERTY(BlueprintReadOnly, Replicated)
	int PlayerCount;

	UFUNCTION(BlueprintCallable)
	void OnRep_UpdateControllingPlayers(TArray<int> inArray);
	UFUNCTION(BlueprintCallable)
	void OnRep_UpdateCharacters(TArray<int> inArray);
	UFUNCTION(BlueprintCallable)
	void OnRep_UpdateLocks(TArray<bool> inArray);

	void OnRep_UpdateUI() { OnUpdateUI.Broadcast(); }
	
};
