
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Utility/Logging/CLogManager.h"
#include "CAction.generated.h"

class UCConsideration;
class UCAction;
class UWorld;
class UCActionComponent;
class ACGridTile;

#pragma region FAbility struct

USTRUCT(BlueprintType)
struct FAbility
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UCAction>> Actions;

	UPROPERTY(Transient)
	TArray<UCAction*> InstantiatedActions;

	UPROPERTY(Transient, meta=(Categories="ItemSlot")) //Is ok if transient + category?
	FGameplayTag InventorySlotTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) //Changed from editdefaults to transient
	FGameplayTagContainer AbilityTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) //Changed from editdefaults to transient
	FGameplayTagContainer BlockingTags;

	//Used for AI
	UPROPERTY(EditDefaultsOnly, Category = "AI") //Will be moved
	TArray<UCConsideration*> Considerations;

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

#pragma endregion

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActionCompleted);

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

	// Tags that are granted to the ability when it is instantiated.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActionTags;

	// Action can only start if OwningActor has none of these Tags applied.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActionBlockingTags;

	//Action nickname to start/stop without a reference to the object.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Action")
	FGameplayTag ActivationTag;

	
public:

	//If true, this was the first action in a chain of events resulting from player input.
	//When undoing actions, we iterate backwards in the action history, until we find one where
	//this is true.
	UPROPERTY()
	bool bIsUserIncited = false;

	//Used to keep track of item charges
	UPROPERTY()
	FGameplayTag UserIncitedItemSlot;

	UPROPERTY(ReplicatedUsing=OnRep_bIsUndone)
	bool bIsUndone = false;

	UFUNCTION()
	void OnRep_bIsUndone();

	void Initialize(UCActionComponent* NewActionComp);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void UndoAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Action")
	static TArray<ACGridTile*> GetAbilityValidTargetTiles(FAbility& Ability, ACGridTile* fromTile) { return Ability.GetValidTargetTiles(fromTile); }

	UPROPERTY(BlueprintAssignable, Category = "Action")
	FOnActionCompleted OnActionCompleted;
	
	virtual UWorld* GetWorld() const override;

	FGameplayTagContainer GetActionTags() const { return ActionTags; }

	FGameplayTagContainer GetBlockedTags() const { return ActionBlockingTags; }

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}
};

