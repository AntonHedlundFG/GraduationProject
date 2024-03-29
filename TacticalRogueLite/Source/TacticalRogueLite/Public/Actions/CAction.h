
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "CAction.generated.h"

class UCAction;
class UWorld;
class UCActionComponent;
class ACGridTile;

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
		bIsRunning = false;
	}
};*/

USTRUCT(BlueprintType)
struct FAbility
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UCAction>> Actions;

	UPROPERTY(VisibleAnywhere)
	TArray<UCAction*> InstantiatedActions;

	UPROPERTY(EditAnywhere)
	FGameplayTag InventorySlotTag;

	TArray<ACGridTile*> GetValidTargetTiles(ACGridTile* fromTile);
	bool IsValidTargetTile(ACGridTile* fromTile, ACGridTile* toTile);

	bool operator==(const FAbility& Other) const {
		return Actions == Other.Actions && InventorySlotTag == Other.InventorySlotTag;
	}

};

inline bool IsValid(const FAbility& Ability)
{
	if (Ability.Actions.Num() == 0)
	{
		return false;
	}
	return true;
}

inline uint32 GetTypeHash(const FAbility& Ability)
{
	uint32 Hash = 0;

	Hash = HashCombine(Hash, GetTypeHash(Ability.InventorySlotTag));

	for (TSubclassOf<UCAction> Action : Ability.Actions)
	{
		Hash = HashCombine(Hash, GetTypeHash(Action));
	}
	
	return Hash;
}


UCLASS(Blueprintable)
class TACTICALROGUELITE_API UCAction : public UObject
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(Replicated)
	TObjectPtr<UCActionComponent> ActionComp;

	UFUNCTION(BlueprintCallable, Category = "Action")
	UCActionComponent* GetOwningComponent() const;

	//Tags added to owning actor when activated, removed when action stops.
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	//Action can only start if OwningActor has none of these Tags applied.
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	// UPROPERTY(ReplicatedUsing="OnRep_RepData")
	// FActionRepData RepData;

	UPROPERTY(Replicated)
	float TimeStarted;

	// UFUNCTION()
	// void OnRep_RepData();

	//Start immediately when added to an action component.
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;

	//Action nickname to start/stop without a reference to the object.
	// UPROPERTY(EditDefaultsOnly, Category = "Action")
	// FGameplayTag ActivationTag;
	
public:
	
	// UFUNCTION(BlueprintPure)
	// FGameplayTag GetActivationTag() const
	// {
	// 	return ActivationTag;
	// }

	UFUNCTION(BlueprintPure)
	bool IsAutoStart() const
	{
		return bAutoStart;
	}

	void Initialize(UCActionComponent* NewActionComp);

	// UFUNCTION(BlueprintCallable, Category = "Action")
	// bool IsRunning() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void UndoAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	virtual UWorld* GetWorld() const override;

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}
};

