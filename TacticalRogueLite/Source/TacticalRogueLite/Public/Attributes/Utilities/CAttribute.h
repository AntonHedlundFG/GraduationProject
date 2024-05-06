// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "CAttribute.generated.h"


//TODO: Ask if reimpl floats.

//Gameplay Attribute to specify health, movement, attackdamage, armor etc.
//Base value is used as the origin to apply the delta to, useful to know for UI to color text red/green as Delta is used for temporary buffs.
//Attributes can be used as modifiers, eg. AttackDamageModifier where the default should be 1 and any Delta (eg. 0.1 = 10%) specified the modifier.

USTRUCT(BlueprintType)
struct FAttribute
{
	GENERATED_BODY()

public:

	FAttribute(int DefaultValue)
	{
		BaseValue = DefaultValue;
		Delta = 0;
		Multiplier = 1;
		bClampToZero = true;
	}

	FAttribute()
	{
		BaseValue = 0;
		Delta = 0;
		Multiplier = 1;
		bClampToZero = true;
	}

	int GetValue() const
	{
		int Value = (BaseValue + Delta) * Multiplier; //?
		
		if (bClampToZero)
	 	{
	 		return FMath::Max(0, Value);
	 	}
	 	//Raw.
	 	return Value;
	 }

	//Eg. 4 Armor.
	UPROPERTY(EditDefaultsOnly, SaveGame, BlueprintReadWrite)
	int BaseValue;

	//Delta added to BaseValue. Eg. An helmet gives you +2.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AdvancedDisplay))
	int Delta;

	//Buffs/Debuffs. Eg. 10 % bonus thanks to some charm.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AdvancedDisplay))
	int Multiplier;

	//Should automatically clamp total value to zero (individual stored base/delta can still be negative).
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AdvancedDisplay))
	bool bClampToZero;
	
};

USTRUCT(BlueprintType)
struct FAttributeCalculationParams
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	class UCActionComponent* SourceComp;

	UPROPERTY(BlueprintReadOnly)
	class UCActionComponent* TargetComp;

	//FGameplayTagContainer SourceTags; 

	//The attribute we are evaluating.
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag AttributeTag;
	
};

UENUM(BlueprintType)
enum class EAttributeModifierOperation : uint8
{
	AddDelta,

	//Adds percentage based modifier which is applied on the (base+delta)*modifier.
	AddMultiplier,

	AddBase,

	OverrideBase,

	//Used when triggering initial event when hooking up a listener.
	Invalid
};

//Simple mapping to modify an Attribute when the ActionEffect is applied.
USTRUCT(BlueprintType)
struct FAttributeModification
{
	GENERATED_BODY()

public:
	FAttributeModification()
	{
		ModifierOperation = EAttributeModifierOperation::AddDelta;
		Magnitude = 0;
		bIsUndo = false;
	}

	int GetMagnitude(int32 Level = 0)const
	{
		//Apply if we have atleast one key.
		if (Curve.GetRichCurveConst() && Curve.GetRichCurveConst()->GetNumKeys() > 0)
		{
			return Magnitude * Curve.GetRichCurveConst()->Eval(Level);
		}
		return Magnitude;
	}

	//The responsible action component that requests this attribute change (can be null).
	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<UCActionComponent> InstigatorComp;

	//Identifier tag to match with attribute.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AttributeTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EAttributeModifierOperation ModifierOperation;

	//The amount to apply, may be multiplied by the Curve variable if set.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int Magnitude;

	//Any tags that can provide context(blocked, crit..).
	UPROPERTY(BlueprintReadWrite)
	FGameplayTagContainer AddedTags;

	//Optional curve, sampled on X-axis at the "Level" of the effect owning this mod. Then multiplied by magnitude.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FRuntimeFloatCurve Curve;

	//EditDefaultsOnly, BlueprintReadOnly, meta = (AdvancedDisplay)
	//Is this modification part of an undo.
	UPROPERTY(BlueprintReadWrite, meta = (EditCondition = false))
	bool bIsUndo;
	
};

USTRUCT(BlueprintType)
struct FAttributeModifications
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
	TArray<FAttributeModification> Modifications;
};







UCLASS()
class TACTICALROGUELITE_API UCAttribute : public UObject
{
	GENERATED_BODY()
	
};
