// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CConsequence.generated.h"

/** UCConsequence is an event that trigger as a -result- of using a command, 
 * rather than a deliberate action from a Controller. 
 * It should be created and registered in the CGameMode with
 * RegisterAndExecuteConsequence(). Do not execute it manually. 
 */
UCLASS(Abstract)
class TACTICALROGUELITE_API UCConsequence : public UObject
{
	GENERATED_BODY()

public:
	virtual void ExecuteConsequence()
	{
		ReceiveExecuteConsequence();
	}
	virtual void UndoConsequence() { ReceiveUndoConsequence(); }
	virtual FString ToString() { return ReceiveToString(); }

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveExecuteConsequence();
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveUndoConsequence();
	UFUNCTION(BlueprintImplementableEvent)
	FString ReceiveToString();

};
