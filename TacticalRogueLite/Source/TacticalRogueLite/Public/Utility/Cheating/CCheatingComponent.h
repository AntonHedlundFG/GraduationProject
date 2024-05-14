// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CCheatingComponent.generated.h"

class ACGameMode;
class ACGameState;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TACTICALROGUELITE_API UCCheatingComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
#if !UE_BUILD_SHIPPING

	virtual void BeginPlay() override;
#endif
	ACGameMode* GameMode;
	ACGameState* GameState;

#pragma region Many Item Charges

public:

	UFUNCTION(BlueprintCallable, Category = "Cheating")
	void SetManyItemCharges(bool bEnabled = true);

	UFUNCTION(BlueprintCallable, Category = "Cheating")
	bool HasManyItemCharges() { return bHasManyItemCharges; }

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Cheating")
	bool bStartWithManyItemCharges = false;
	bool bHasManyItemCharges = false;
	int32 ManyItemChargesAmount = 100;
	
#pragma endregion
	
#pragma region Big Health

public:

	UFUNCTION(BlueprintCallable, Category = "Cheating")
	void GainBigHealth();

protected:

	int32 BigHealthAmount = 100;



#pragma endregion

#pragma region Win This Turn

public:

	UFUNCTION(BlueprintCallable, Category = "Cheating")
	void WinThisTurn();

	UFUNCTION(BlueprintCallable, Category = "Cheating")
	void LoseThisTurn();

#pragma endregion

};