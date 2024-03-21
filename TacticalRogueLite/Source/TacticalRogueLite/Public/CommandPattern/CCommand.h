// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CCommand.generated.h"

/**
 * 
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

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveExecuteCommand();
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveUndoCommand();
	UFUNCTION(BlueprintImplementableEvent)
	FString ReceiveToString();

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AController> CommandCreator;

};
