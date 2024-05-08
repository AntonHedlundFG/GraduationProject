
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Utilities/CAttributeUtilities.h"
#include "CAttributeComponent.generated.h"

//Säga till visualeffectcomp att något har ändrats(repl relaterat)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActiveGamePlayTagsChanged);
//Alternative: Share the same signature with generic names.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, AActor*, InstigatorActor, UCAttributeComponent*, OwningComp, int, NewValue, int, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemChargesChanged);

UCLASS(BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TACTICALROGUELITE_API UCAttributeComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static UCAttributeComponent* GetAttributes(AActor* FromActor);

	//Array of gameplaytags. Contains useful utility function on it like "HasTag", "HasAnyOfTheseTags".
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicatedusing = OnRep_ActiveGameplayTags, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;

	UFUNCTION()
	void OnRep_ActiveGameplayTags()
	{
		OnActiveGamePlayTagsChanged.Broadcast();
	}

	UPROPERTY(BlueprintAssignable)
	FOnActiveGamePlayTagsChanged OnActiveGamePlayTagsChanged;
	
	UCAttributeComponent();

public:
	UPROPERTY(Replicated)
	FGameplayTagStackContainer GameplayStats;

	void AddStatTagCount(FGameplayTag Tag, int32 Count) { return GameplayStats.AddStackCount(Tag, Count); }
	
	void RemoveStatTagCount(FGameplayTag Tag, int32 Count) { return GameplayStats.RemoveStackCount(Tag, Count); }
	
	int32 GetStatTagStackCount(FGameplayTag Tag) const { return GameplayStats.GetStackCount(Tag); }
 
	bool HasStatTag(FGameplayTag Tag) const { return GameplayStats.HasTag(Tag); }

#pragma region Item Charges

public:

	UFUNCTION(BlueprintCallable, Category = "Abilities|Charges")
	bool HasRemainingCharges(FGameplayTag ItemSlot, int32 Amount = 1) { 
		return GetRemainingCharges(ItemSlot) >= Amount; 
	}

	UFUNCTION(BlueprintCallable, Category = "Abilities|Charges")
	int32 GetRemainingCharges(FGameplayTag ItemSlot);

	UFUNCTION(BlueprintCallable, Category = "Abilities|Charges")
	bool TrySpendCharge(FGameplayTag ItemSlot, int32 Amount = 1);

	UFUNCTION(BlueprintCallable, Category = "Abilities|Charges")
	bool TryUndoSpendCharge(FGameplayTag ItemSlot, int32 Amount = 1);

	UFUNCTION(BlueprintCallable, Category = "Abilities|Charges")
	void AddMaxCharges(FGameplayTag ItemSlot, int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Abilities|Charges")
	void RemoveMaxCharges(FGameplayTag ItemSlot, int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Abilities|Charges")
	void ResetSpentCharges();

	UPROPERTY(BlueprintAssignable, Category = "Abilities|Charges")
	FOnItemChargesChanged OnItemChargesChanged;


protected:

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Abilities|Charges")
	FGameplayTagStackContainer UsedItemCharges;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Abilities|Charges")
	FGameplayTagStackContainer MaxItemCharges;

	UFUNCTION()
	void BroadcastOnItemChargesChanged();

#pragma endregion
	
	
};
