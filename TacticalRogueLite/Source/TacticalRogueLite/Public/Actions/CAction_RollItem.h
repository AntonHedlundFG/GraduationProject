// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction.h"
#include "CAction_RollItem.generated.h"

class UCItemData;
struct FBucketInfo;

/*USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	TObjectPtr<AActor> Instigator;

	FActionRepData()
	{
		bIsRunning = true;
	}
}*/;

UCLASS()
class TACTICALROGUELITE_API UCAction_RollItem : public UCAction
{
	GENERATED_BODY()

protected:

	// UPROPERTY(ReplicatedUsing="OnRep_RepData")
	// FActionRepData RepData;

	// UFUNCTION()
	// void OnRep_RepData();
	
	UPROPERTY(Replicated)
	TArray<UCItemData*> Items;

	//What DataTable to use when rolling items.
	UPROPERTY(EditDefaultsOnly, Category = "LootTable")
	UDataTable* LootTable;
	//What "category" to roll on. Charms/Defaults. TODO: Explain.
	UPROPERTY(EditDefaultsOnly, Category = "LootTable", DisplayName = "Loot Category")
	TArray<FBucketInfo> Buckets;

	//Amount of items to retrieve.
	UPROPERTY(EditDefaultsOnly, Category = "LootTable")
	int RollAmount = 3;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:

	virtual void StartAction(AActor* Instigator) override;
	virtual void StopAction(AActor* Instigator) override;
	virtual void UndoAction(AActor* Instigator) override;

	// UFUNCTION(BlueprintCallable, Category = "Action")
	// bool IsRunning() const;

	TArray<UCItemData*> GetItems();
	TObjectPtr<AActor> GetInstigator(){ return RepData.Instigator; }
};
