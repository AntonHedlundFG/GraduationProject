
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Utility/Logging/CLogManager.h"
#include "CAction.generated.h"

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

	UPROPERTY(VisibleAnywhere)
	TArray<UCAction*> InstantiatedActions;

	UPROPERTY(EditAnywhere)
	FGameplayTag InventorySlotTag;

	UPROPERTY(VisibleAnywhere)
	FGameplayTagContainer ActionGameplayTags;

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

	// GrantToAbilityTags
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	// Action can only start if OwningActor has none of these Tags applied.
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags; // BlockingTags

	
	
public:

	//If true, this was the first action in a chain of events resulting from player input.
	//When undoing actions, we iterate backwards in the action history, until we find one where
	//this is true.
	UPROPERTY()
	bool bIsUserIncited = false;

	void Initialize(UCActionComponent* NewActionComp);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void UndoAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	virtual UWorld* GetWorld() const override;

	FGameplayTagContainer GetGrantsTags() const { return GrantsTags; }

	FGameplayTagContainer GetBlockedTags() const { return BlockedTags; }

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}
};

