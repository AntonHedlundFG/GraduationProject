// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTags.h"
#include "Attributes/CAttributeComponent.h"
#include "GridContent/CUnit.h"
#include "CItem.generated.h"

class UCCommand;
class ACUnit;
class ACGridTile;

/** If making a C++ extension that is also extended by AngelScript/Blueprints, 
* make sure you call the Super::GenerateAbilityCommand() function manually. 
* If the implementation is entirely C++, do not make a Super:: call.
 */
UCLASS(Abstract)
class TACTICALROGUELITE_API UCItem : public UDataAsset
{
	GENERATED_BODY()

protected:
	
	//Tags added to owning actor when item is equipped.
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	//The item can not create an executable command if owningactor has any of these tags applied.
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	//Item "nickname" to start/stop without a reference to the object. EXPERIMENTAL
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	FGameplayTag ActivationTag;

	//Start immediately when added to a attribute component. EXPERIMENTAL- change to more turn based manner
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	bool bAutoStart;
	
	UPROPERTY() //Replicated
	int TurnCounterStarted;

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UTexture2D> AbilityIcon;
	
	
	virtual UCCommand* GenerateAbilityCommand(AController* inController, ACUnit* inUnit, ACGridTile* inTargetTile)
	{
		return ReceiveGenerateAbilityCommand(inController, inUnit, inTargetTile);
	}

	UFUNCTION(BlueprintImplementableEvent)
	UCCommand* ReceiveGenerateAbilityCommand(AController* inController, ACUnit* inUnit, ACGridTile* inTargetTile);

	virtual TArray<ACGridTile*> GetReachableTiles(ACGridTile* inTile) 
	{ 
		return ReceiveGetReachableTiles(inTile); 
	}

	UFUNCTION(BlueprintImplementableEvent)
	TArray<ACGridTile*> ReceiveGetReachableTiles(ACGridTile* inTile); 

	TArray<ACGridTile*> GetValidTargetTiles(ACUnit* inUnit) 
	{
		if (UCAttributeComponent::GetAttributes(inUnit)->ActiveGameplayTags.HasAny(BlockedTags))
		{
			return TArray<ACGridTile*>();
		}
			
		return GetValidTargetTiles(inUnit);
	}
	
	
	virtual TArray<ACGridTile*> GetValidTargetTilesInternal(ACUnit* inUnit) //Override in C++. 
	{
		return ReceiveGetValidTargetTiles(inUnit); 
	}

	UFUNCTION(BlueprintImplementableEvent)
	TArray<ACGridTile*> ReceiveGetValidTargetTiles(ACUnit* inUnit); //Override in AS.

	void EquipOnUnit(ACUnit* inUnit)
	{
		//UCAttributeComponent::GetAttribute(inUnit)->AppendTags(GrantsTags);
		EquipOnUnitInternal(inUnit);
	}

	virtual void EquipOnUnitInternal(ACUnit* inUnit)
	{
		ReceiveEquipOnUnit(inUnit);
	}

	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveEquipOnUnit(ACUnit* inUnit);


	

	UFUNCTION(BlueprintCallable, Category = "Abilities|Targeting")
	bool IsValidTargetTile(ACUnit* inUnit, ACGridTile* inTargetTile);

	/* Removed, we should just be using ACGridTile* instead of int indexes
	UFUNCTION(BlueprintCallable, Category = "Abilities|Targeting")
	bool IsValidTargetTileIndex(ACUnit* inUnit, const int inTargetTileIndex);
	*/

};