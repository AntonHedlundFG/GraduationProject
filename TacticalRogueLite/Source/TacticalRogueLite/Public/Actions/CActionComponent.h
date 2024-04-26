#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "CAction.h"
#include "CActionWithTimer.h"
#include "Attributes/Utilities/CAttribute.h"
#include "CActionComponent.generated.h"

//class UCAction;

class UCAttributeSet;

//Triggers on an attribute change inside Attributeset. Difference in value, Contexttags= Whatever you need- Specify who triggered this? Damage.Block? Damage.Crithit? Gives _context_.
DECLARE_DYNAMIC_DELEGATE_SevenParams(FAttributeChangedSignature, UCActionComponent*, ActionComp, UCActionComponent*, InstigatorComp, FGameplayTag, AttributeTag, int, NewValue, int, Delta, const FGameplayTagContainer&, ContextTags, EAttributeModifierOperation, ModType);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionStateChanged, UCActionComponent*, OwningComp, UCAction*, Action);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityChanged, UCActionComponent*, OwningComp, FAbility, Ability);

UENUM(BlueprintType)
enum class EGameplayEventScope : uint8
{
	//Only listen for events caused by ActionComp itself or directly triggered on the component.
	LocalOnly,

	//Only listen to global triggers, eg. if any pawn is killed in the world, we can react to it.
	GlobalOnly,

	//Listen for any event scope.
	Both
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TACTICALROGUELITE_API UCActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Actions")
	static UCActionComponent* GetActionComp(AActor* FromActor);

	//Array of gameplaytags. Contains useful utility function on it like "HasTag", "HasAnyOfTheseTags".
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void AddAction(AActor* Instigator, TSubclassOf<UCAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void RegisterAction(UCAction* NewAction);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void RemoveAction(UCAction* ActionToRemove);

	//Returns first occurrence of action matching the class provided #1#.
	UFUNCTION(BlueprintCallable, Category = "Actions")
	UCAction* GetAction(TSubclassOf<UCAction> ActionClass) const;

	// UFUNCTION(BlueprintCallable, Category = "Actions")
	// bool StartActionByName(AActor* Instigator, FGameplayTag ActionName);
	
	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StopActionByName(AActor* Instigator, FGameplayTag ActionName);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void AddAbility(FAbility Ability);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void RemoveAbility(FGameplayTag ItemSlot);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool TryGetAbility(FGameplayTag ItemSlot, FAbility& outAbility);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	TArray<ACGridTile*> GetValidTargetTiles(FGameplayTag itemSlot);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void ToggleHighlightOnValidTargetTiles(FGameplayTag itemSlot, ACGridTile* Tile, bool bHighlightOn);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool IsValidTargetTile(FGameplayTag ItemSlot, class ACGridTile* TargetTile);

	UCActionComponent();

	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionStarted;

	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionStopped;

	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionUndo;
	
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

protected:

	UPROPERTY(BlueprintReadOnly, Replicated)
	TArray<TObjectPtr<UCAction>> Actions;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	TArray<FAbility> Abilities;
	
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	
	// -- Attributes -- //
	
	//Attribute set for things like Health, HealthMax...
	UPROPERTY(Replicated)
	UCAttributeSet* AttributeSet;

	bool GetAttribute(FGameplayTag InTag, FAttribute& InAttribute);
	
	//1
	//Static blueprint accessor for all data contained inside all FAttribute, so never have to deal with structs.
	UFUNCTION(BlueprintPure, Category = "Attributes", meta =(DisplayName = "Get Attribute"))
	bool K2_GetAttribute(FGameplayTag AttributeTag, int& CurrentValue, int& BaseValue, int &Additive, int& Multiplier);

	
	//Apply a simple modification to an attribute (eg. change HealthMax or adding Armor value).
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Attributes", meta = (AdvancedDisplay = "Level", Keywords = "Add, Set"))
	int ApplyAttributeChange(const FAttributeModification& InAttributeMod, int32 Level /*=0*/);

	
	//Check if attributes on Target will remain above or equal to zero if modifiers get applied.
	bool CanApplyAttributeModifiers(UCActionWithTimer* Effect);
	
	UFUNCTION(BlueprintCallable, Category = "Game|Attributes", meta = (AutoCreateRefTerm = "Event", DisplayName = "Add Attribute Listener", AdvancedDisplay = "bCallNow", Keywords = "Assign, Create"))
	void AddAttributeChangedListener(FGameplayTag AttributeTag, const FAttributeChangedSignature& Event, bool bCallNow);
	
	//Mapping for attribute change listeners. List of changed triggers. Whenever we make a change to our attributeset, we call "ApplyAttributeChange" on AttributeSet, which will broadcast to anyone whos interested, and also call "BroadcastAttributeChanged" here. 
	TArray<TPair<FGameplayTag, FAttributeChangedSignature>> AttributeChangeTriggers;
	
	void BroadcastAttributeChanged(FGameplayTag InAttributeTag, UCActionComponent* InstigatorComp, int InNewValue, int InDelta, FGameplayTagContainer InContextTags, EAttributeModifierOperation ModOperation);

protected:

	
	UPROPERTY(EditDefaultsOnly, NoClear, Category = "Attributes")
	TSubclassOf<UCAttributeSet> AttributeClass;
	
	
		
};
