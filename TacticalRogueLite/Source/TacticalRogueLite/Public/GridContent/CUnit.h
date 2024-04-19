
#pragma once

#include "CoreMinimal.h"
#include "GridContent/CGridContent.h"
#include "CUnit.generated.h"

struct FAbility;
class UCActionComponent;
class UCAttributeComponent;
class UCInventoryComponent;
class UCItem;
class ACGridTile;
enum class EItemSlots : uint8;

UCLASS()
class TACTICALROGUELITE_API ACUnit : public ACGridContent
{
	GENERATED_BODY()


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpriteMesh")
	USceneComponent* SceneComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCAttributeComponent> AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCActionComponent> ActionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCInventoryComponent> InventoryComp;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Units")
	FString UnitName = FString();
	
public:
	ACUnit();
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;
	UFUNCTION(BlueprintCallable)
	FString GetUnitName() { return UnitName; }
	UFUNCTION(BlueprintCallable)
	void SetUnitName(FString inName) { UnitName = inName; }

	// This determines who can control this unit. 0 means AI
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	uint8 ControllingPlayerIndex = 0;

	UFUNCTION(BlueprintCallable)
	bool IsControlledBy(AController* inController);
	
	UFUNCTION(BlueprintCallable)
	UCItemData* GetItemDataInSlot(FGameplayTag inSlot);
	
	UFUNCTION(BlueprintCallable)
	bool TryGetAbilityInSlot(FGameplayTag ItemSlot, FAbility& outAbility);

	UFUNCTION(BlueprintCallable)
	TArray<FAbility> GetEquippedAbilities() const;
	
	UFUNCTION(BlueprintCallable)
	UCAttributeComponent* GetAttributeComp() const { return AttributeComp; }
	
	UFUNCTION(BlueprintCallable)
	UCActionComponent* GetActionComp() const { return ActionComp; }

	UFUNCTION(BlueprintCallable)
	UCInventoryComponent* GetInventoryComp() const { return InventoryComp; }

	
	// ----------------------------------------------------------------
	
#pragma region Item Charges

public:

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool HasRemainingCharges(FGameplayTag ItemSlot, int32 Amount = 1);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	int32 GetRemainingCharges(FGameplayTag ItemSlot);

#pragma endregion


};
