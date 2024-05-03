// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Attributes/Utilities/CAttribute.h"
#include "UObject/NoExportTypes.h"
#include "CAttributeSet.generated.h"


class UCActionComponent;

//Subclass in game code to add additional attributes.

UCLASS(Blueprintable)
class TACTICALROGUELITE_API UCAttributeSet : public UObject
{
	GENERATED_BODY()

public:

	
	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}

	UCAttributeSet();

	virtual void Initialize(UCActionComponent* NewOwner);

	UFUNCTION(BlueprintCallable, Category = "Action")
	UCActionComponent* GetOwningComponent() const;

	UFUNCTION(BlueprintImplementableEvent)
	void K2_Initialize();

	bool GetAttribute(FGameplayTag InTag, FAttribute& OutAttribute);
	
	//Direct access to modifying attributes.
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Attributes", meta = (AdvancedDisplay="Level"))
	int ApplyAttributeChange(const FAttributeModification& ModToApply, int32 Level = 0);

	UFUNCTION(BlueprintNativeEvent)
	void PostAttributeChanged(const FAttributeModification& AppliedMod, int32 Level = 0);

	//Current Health value, clamped between 0 and (HealthMax * HealthMaxModifier).?
	UPROPERTY(EditAnywhere, SaveGame, ReplicatedUsing=OnRep_Health, BlueprintReadOnly, Category = "Attributes")
	FAttribute Health;

	//Maximum Health including any flat modification, can be _changed_ by upgrades and buffs.
	UPROPERTY(EditAnywhere, SaveGame, ReplicatedUsing=OnRep_HealthMax, BlueprintReadOnly, Category = "Attributes")
	FAttribute HealthMax;

	//Percentage modifier for maximum health used by _temporary_ upgrades and buffs.
	UPROPERTY(EditAnywhere, SaveGame, ReplicatedUsing=OnRep_HealthModifier, BlueprintReadOnly, Category = "Attributes")
	FAttribute HealthMaxModifier;

	//Current Health value, clamped between 0 and (HealthMax * HealthMaxModifier).?
	UPROPERTY(EditAnywhere, SaveGame, ReplicatedUsing=OnRep_AttackDamage, BlueprintReadOnly, Category = "Attributes")
	FAttribute AttackDamage;

	//Maximum Health including any flat modification, can be _changed_ by upgrades and buffs.
	UPROPERTY(EditAnywhere, SaveGame, ReplicatedUsing=OnRep_AttackRange, BlueprintReadOnly, Category = "Attributes")
	FAttribute AttackRange;

	//Percentage modifier for maximum health used by _temporary_ upgrades and buffs.
	UPROPERTY(EditAnywhere, SaveGame, ReplicatedUsing=OnRep_MovementRange, BlueprintReadOnly, Category = "Attributes")
	FAttribute MovementRange;

	//Percentage modifier for maximum health used by _temporary_ upgrades and buffs.
	UPROPERTY(EditAnywhere, SaveGame, ReplicatedUsing=OnRep_Armor, BlueprintReadOnly, Category = "Attributes")
	FAttribute Armor;

	UFUNCTION()
	void OnRep_Health(FAttribute OldAttribute);
	
	UFUNCTION()
	void OnRep_AttackDamage(FAttribute OldAttribute);

	UFUNCTION()
	void OnRep_AttackRange(FAttribute OldAttribute);

	UFUNCTION()
	void OnRep_Armor(FAttribute OldAttribute);

	UFUNCTION()
	void OnRep_MovementRange(FAttribute OldAttribute);
	
	UFUNCTION()
	void OnRep_HealthMax(FAttribute OldAttribute);

	UFUNCTION()
	void OnRep_HealthModifier(FAttribute OldAttribute);

	//Helper function for every RepNotify to trigger attribute events on clients.
	UFUNCTION(BlueprintCallable)
	void AttributeChangedFromReplication(FName AttributeName, FAttribute NewAttribute, FAttribute OldAttribute);

	//Special Access to the game world from UObjects.
	//UWorld* GetWorld() const override;

protected:
	
	virtual UWorld* GetWorld() const override;

	//Returns current value of the attribute.
	UFUNCTION(BlueprintPure, Category = "AttributeSet")
	int GetAttributeValue(FGameplayTag InTag);
	
	bool GetAttributeName(FGameplayTag InTag, FName& OutAttributeName);

	UPROPERTY(ReplicatedUsing=OnRep_OwningComp)
	UCActionComponent* OwningComp;

	UFUNCTION()
	void OnRep_OwningComp();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	

	

	
};
