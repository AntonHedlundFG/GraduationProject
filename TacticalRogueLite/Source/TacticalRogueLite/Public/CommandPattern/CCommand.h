// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CCommand.generated.h"

class UCConsequence;

/** Commands should not be created and executed manually. Instead, you should
 * override the GenerateAbilityCommand() of a UCItem to create the command, which
 * will then be executed and undone automatically by the GameMode.
 * Note that things that trigger as a -result- of using a command, rather than a 
 * deliberate action from a Controller, should be created as a UCConsequence
 * and registered in the CGameMode with RegisterAndExecuteConsequence()
 */
UCLASS(Abstract)
class TACTICALROGUELITE_API UCCommand : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void ExecuteCommand(AController* inController) 
	{ 
		CommandCreator = inController;
		ReceiveExecuteCommand();
	}
	virtual void UndoCommand() { ReceiveUndoCommand(); }
	virtual FString ToString() { return ReceiveToString(); }

	UFUNCTION(BlueprintCallable, Category = "Abilities|Commands")
	AController* GetCommandCreator() { return CommandCreator.Get(); }

	void StoreConsequence(UCConsequence* inConsequence);
	void UndoAllConsequences();

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveExecuteCommand();
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveUndoCommand();
	UFUNCTION(BlueprintImplementableEvent)
	FString ReceiveToString();

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AController> CommandCreator;

	UPROPERTY()
	TArray<UCConsequence*> Consequences;

};
