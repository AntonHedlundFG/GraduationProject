
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "CAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, UCAttributeComponent*, OwningComp, int, NewHealth, int, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnRageChanged, AActor*, InstigatorActor, UCAttributeComponent*, OwningComp, int, NewRage, int, Delta);

//Säga till visualeffectcomp att något har ändrats(repl relaterat)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActiveGamePlayTagsChanged);
//Alternative: Share the same signature with generic names.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, AActor*, InstigatorActor, UCAttributeComponent*, OwningComp, int, NewValue, int, Delta);


UCLASS(BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TACTICALROGUELITE_API UCAttributeComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	
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
	
	

	UFUNCTION(BlueprintCallable, Category = "Attributes", meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);

	UCAttributeComponent();


protected:
	//movement turns
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing=OnRep_CurrentHealth, Category = "Attributes")
	int CurrentHealth;

	UFUNCTION()
	void OnRep_CurrentHealth()
	{
		//Handle visual stuff based on CurrentHealth
	}
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	int BaseHealth;

	/*Resource used to power certain Actions*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float Rage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float RageMax;

	//UPROPERTY(ReplicatedUsing="")
	//bool bIsAlive;

	UFUNCTION(NetMulticast, Reliable) //NOTE: could mark as unreliable once moving the 'state' out of character(eg. once its cosmetic only).
	void MulticastHealthChanged(AActor* InstigatorActor, int NewHealth, int Delta);


public:

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool Kill(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsFullHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	int GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void SetHealth(int NewHealth);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	int GetBaseHealth() const;

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttributeChanged OnRageChanged;

	//Passing in the difference we want applied. Return if the change actually succeeded.
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, int Delta);
	

#pragma region Item Charges

public:

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool HasRemainingCharges(FGameplayTag ItemSlot, int Amount = 1) { 
		return RemainingCharges(ItemSlot) >= Amount; 
	}

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	uint8 RemainingCharges(FGameplayTag ItemSlot);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool TrySpendCharge(FGameplayTag ItemSlot, uint8 Amount = 1);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool TryUndoSpendCharge(FGameplayTag ItemSlot, uint8 Amount = 1);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void AddMaxCharges(FGameplayTag ItemSlot, uint8 Amount);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void RemoveMaxCharges(FGameplayTag ItemSlot, uint8 Amount);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Abilities")
	uint8 BootsCharges = 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Abilities")
	uint8 WeaponCharges = 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Abilities")
	uint8 ArmorCharges = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Abilities")
	uint8 TrinketCharges = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Abilities")
	uint8 BootsUsed = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Abilities")
	uint8 WeaponUsed = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Abilities")
	uint8 ArmorUsed = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Abilities")
	uint8 TrinketUsed = 0;

	uint8& GetMaxChargesRef(FGameplayTag ItemSlot);
	uint8& GetUsedChargesRef(FGameplayTag ItemSlot);

#pragma endregion
	
	
};
