
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Attributes/Utilities/CAttribute.h"
#include "Grid/Tiles/TileHighlightModes.h"
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = false)) 
	FGameplayTagContainer AbilityTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) 
	FGameplayTagContainer BlockingTags;
	
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UCAction>> Actions;

	UPROPERTY(Transient, BlueprintReadOnly)
	TArray<UCAction*> InstantiatedActions;

	UPROPERTY(Transient, meta=(Categories="ItemSlot")) 
	FGameplayTag InventorySlotTag;
	
	// Considerations for AI to evaluate the ability
	UPROPERTY(EditDefaultsOnly, Category = "AI") //Will be moved
	TArray<UCConsideration*> Considerations;

	TSet<ACGridTile*> GetInfluencedTiles(ACGridTile* fromTile);
	TArray<ACGridTile*> GetValidTargetTiles(ACGridTile* fromTile);
	bool IsValidTargetTile(ACGridTile* fromTile, ACGridTile* toTile);
	void ToggleHighlightTilesInRange(ACGridTile* fromTile, bool bHighlightOn, bool bToggleAffected = true);
	
	bool operator==(const FAbility& Other) const
	{
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


USTRUCT()
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
		bIsRunning = true; //TODO: originally false but is set to not mess up any previously impl actions.
	}
};


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

	//Start immediately when added to an action component. 
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;

	UPROPERTY(ReplicatedUsing="OnRep_RepData")
	FActionRepData RepData;

	UFUNCTION()
	void OnRep_RepData();

	// Highlight that will show on the tiles affected by the action.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ETileHighlightModes AffectedHighlightMode = ETileHighlightModes::ETHM_NONE;
	
public:

	// List of modifiers to apply to attributes when the action is active.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	TArray<FAttributeModification> ModifiersAppliedToOwner;

	UPROPERTY(BlueprintReadWrite, meta = (EditCondition = false))
	TArray<int> ModifiersActualDeltas;

	UFUNCTION(BlueprintCallable)
	UCActionComponent* GetActionComp() { return ActionComp; }

	UFUNCTION(BlueprintPure)
	bool IsAutoStart() const
	{
		return bAutoStart;
	}
	
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;

	UFUNCTION()
	void OnRep_bIsUndone();

	void Initialize(UCActionComponent* NewActionComp);

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetActivationTag() const { return ActivationTag; }
	

	UFUNCTION(Category = "Action")
	virtual bool CanStart(AActor* Instigator);

	UFUNCTION(Category = "Action")
	virtual void StartAction(AActor* Instigator);

	UFUNCTION(Category = "Action")
	virtual void UndoAction(AActor* Instigator);

	UFUNCTION(Category = "Action")
	virtual void StopAction(AActor* Instigator);

	

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


#pragma region AngelScript events

	// These are necessary as Super::CanStart() cannot be called from AngelScript.
	// It's a known issue, and the suggested workaround is this solution, where we
	// call these Receive-functions from the main functions (CanStart, StartAction, etc)

	UFUNCTION(BlueprintNativeEvent, Category = "Action", meta = (DisplayName = "Can Start"))
	bool ReceiveCanStart(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action", meta = (DisplayName = "Start Action"))
	void ReceiveStartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action", meta = (DisplayName = "Undo Action"))
	void ReceiveUndoAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action", meta = (DisplayName = "Stop Action"))
	void ReceiveStopAction(AActor* Instigator);

#pragma endregion

	// Need to be overridden in child classes that can influence more than one tile at a time. (AoE, etc.)
	// Default implementation just returns the tile the action was started from. (Single target)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	TSet<ACGridTile*> GetActionInfluencedTiles(ACGridTile* fromTile);

	UFUNCTION()
	ETileHighlightModes GetAffectedHighlightMode() const { return AffectedHighlightMode; }
	
	UFUNCTION(BlueprintCallable, Category = "Ability")
	static TSet<ACGridTile*> GetAbilityInfluencedTiles(FAbility& Ability, ACGridTile* fromTile) { return Ability.GetInfluencedTiles(fromTile); }

	UFUNCTION(BlueprintCallable, Category = "Ability")
	static TArray<ACGridTile*> GetAbilityValidTargetTiles(FAbility& Ability, ACGridTile* fromTile) { return Ability.GetValidTargetTiles(fromTile); }
	
	UFUNCTION(BlueprintCallable, Category = "Action")
	static void ToggleHighlightTilesInRange(FAbility Ability, ACGridTile* fromTile, bool bHighlightOn);

	virtual UWorld* GetWorld() const override;

	FGameplayTagContainer GetActionTags() const { return ActionTags; }

	FGameplayTagContainer GetBlockedTags() const { return ActionBlockingTags; }

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}
};


