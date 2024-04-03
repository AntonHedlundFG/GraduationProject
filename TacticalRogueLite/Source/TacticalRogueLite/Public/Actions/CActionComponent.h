#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "CAction.h"
#include "CActionComponent.generated.h"

//class UCAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityChanged, UCActionComponent*, OwningComp, FAbility, Ability);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TACTICALROGUELITE_API UCActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	//Array of gameplaytags. Contains useful utility function on it like "HasTag", "HasAnyOfTheseTags".
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void AddAbility(FAbility Ability);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void RemoveAbility(FGameplayTag ItemSlot);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool TryGetAbility(FGameplayTag ItemSlot, FAbility& outAbility);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	TArray<ACGridTile*> GetValidTargetTiles(FGameplayTag itemSlot);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool IsValidTargetTile(FGameplayTag ItemSlot, class ACGridTile* TargetTile);

	UCActionComponent();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	TArray<FAbility> Abilities;
	
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:

	//TODO: Replace these with relevant delegates
	UPROPERTY(BlueprintAssignable)
	FOnAbilityChanged OnAbilityChanged;

	// UPROPERTY(BlueprintAssignable)
	// FOnActionStateChanged OnCharmAdded;
	
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	
		
};
