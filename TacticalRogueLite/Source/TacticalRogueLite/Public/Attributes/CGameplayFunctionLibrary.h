// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CGameplayFunctionLibrary.generated.h"

struct FGameplayTagStackContainer;
struct FAttributeModifications;

UCLASS()
class TACTICALROGUELITE_API UCGameplayFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	
	// UPROPERTY()
	// int Amount = 0;

	public:

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	static FAttributeModifications ApplyDamage(AActor* DamageCauser, AActor* TargetActor, int DamageAmount, FGameplayTagContainer ContextTags);

	UFUNCTION(BlueprintPure, Category = "FGameplayStackContainer")
	static FGameplayTagContainer ToGameplayTagContainer( const FGameplayTagStackContainer& StackedContainer);
	
	//UFUNCTION(BlueprintCallable,Category="Gameplay")
	//static bool UndoDamage(AActor* InstigatorActor, AActor* TargetActor, int Amount, FGameplayTagContainer ContextTags);
	// UFUNCTION(BlueprintPure, Category = "PSO Caching")
	// static int32 GetRemainingBundledPSOs();

	};

