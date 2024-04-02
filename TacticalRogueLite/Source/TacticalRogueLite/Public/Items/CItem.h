// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTags.h"
#include "ItemSlots.h"
#include "CItem.generated.h"

class UCCommand;
class ACUnit;
class ACGridTile;
class UCAttributeComponent;

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

	/* -- Is there a use case for this? -- 
	//Item "nickname" to start/stop without a reference to the object. EXPERIMENTAL
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	FGameplayTag ActivationTag;
	*/

public:

	//Fetched by UI to display on buttons
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UTexture2D> AbilityIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Slot")
	EItemSlots ItemSlot; 
	
	/// <summary>
	/// Given a unit and a tile, can this item currently be used with
	/// that unit as the active unit, and the tile as the target?
	/// </summary>
	/// <param name="inUnit"> The active unit </param>
	/// <param name="inTargetTile"> The desired target tile </param>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable, Category = "Abilities|Targeting")
	bool IsValidTargetTile(ACUnit* inUnit, ACGridTile* inTargetTile);

	/// <summary>
	/// Given a unit, create an array of all tiles that are valid targets 
	/// for activating the item's ability.
	/// </summary>
	/// <param name="inUnit"> The active unit </param>
	/// <returns> An array of valid target tiles </returns>
	TArray<ACGridTile*> GetValidTargetTiles(ACUnit* inUnit);

	/// <summary>
	/// Called by the server when a unit equips this item. This applies any
	/// tags in GrantsTags to the unit, and EquipOnUnitInternal can be overridden
	/// to apply further custom effects.
	/// </summary>
	/// <param name="inUnit"></param>
	void EquipOnUnit(ACUnit* inUnit);

	/// <summary>
	/// Called by the server when a unit unequips this item. Removes any tags in 
	/// GrantsTags from the unit, and UnequipOnUnitInternal can be overridden to 
	/// remove any further custom effects.
	/// </summary>
	/// <param name="inUnit"></param>
	void UnequipOnUnit(ACUnit* inUnit);



#pragma region C++ overrides

public:

	//virtual UCCommand* GenerateAbilityCommand(AController* inController, ACUnit* inUnit, ACGridTile* inTargetTile)
	//{ return ReceiveGenerateAbilityCommand(inController, inUnit, inTargetTile); }

	virtual TArray<ACGridTile*> GetReachableTiles(ACGridTile* inTile)
	{ return ReceiveGetReachableTiles(inTile); }


protected:

	virtual TArray<ACGridTile*> GetValidTargetTilesInternal(ACUnit* inUnit)
	{ return ReceiveGetValidTargetTiles(inUnit); }

	virtual void EquipOnUnitInternal(ACUnit* inUnit)
	{ ReceiveEquipOnUnit(inUnit); }

	virtual void UnequipOnUnitInternal(ACUnit* inUnit)
	{ ReceiveUnequipOnUnit(inUnit); }

#pragma endregion


#pragma region AngelScript overridables

protected:

	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveEquipOnUnit(ACUnit* inUnit);
	//UFUNCTION(BlueprintImplementableEvent)
	//UCCommand* ReceiveGenerateAbilityCommand(AController* inController, ACUnit* inUnit, ACGridTile* inTargetTile);
	UFUNCTION(BlueprintImplementableEvent)
	TArray<ACGridTile*> ReceiveGetValidTargetTiles(ACUnit* inUnit); 
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveUnequipOnUnit(ACUnit* inUnit);
	UFUNCTION(BlueprintImplementableEvent)
	TArray<ACGridTile*> ReceiveGetReachableTiles(ACGridTile* inTile);

#pragma endregion
	
	

};