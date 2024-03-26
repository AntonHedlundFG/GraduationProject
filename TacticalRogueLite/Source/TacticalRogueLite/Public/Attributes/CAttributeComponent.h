
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "CAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, UCAttributeComponent*, OwningComp, float, NewHealth, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnRageChanged, AActor*, InstigatorActor, UCAttributeComponent*, OwningComp, float, NewRage, float, Delta);

//Säga till visualeffectcomp att något har ändrats(repl relaterat)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActiveGamePlayTagsChanged);
//Alternative: Share the same signature with generic names.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, AActor*, InstigatorActor, UCAttributeComponent*, OwningComp, float, NewValue, float, Delta);


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

	// min container == activegameplaytags(attr)
	

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
	void MulticastHealthChanged(AActor* InstigatorActor, float NewHealth, float Delta);

	UFUNCTION(NetMulticast, Unreliable) // Used for cosmetic changes only
	void MulticastRageChanged(AActor* InstigatorActor, float NewRage, float Delta);

public:

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool Kill(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsFullHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetBaseHealth() const;

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttributeChanged OnRageChanged;

	//Passing in the difference we want applied. Return if the change actually succeeded.
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void ApplyTagWithDuration(FGameplayTag InTag, int Duration);

	// UFUNCTION(BlueprintCallable)
	// float GetRage() const;
	//
	// UFUNCTION(BlueprintCallable, Category = "Attributes")
	// bool ApplyRage(AActor* InstigatorActor, float Delta);

	//Testing:
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	//FRuntimeFloatCurve Curve;
	
};
