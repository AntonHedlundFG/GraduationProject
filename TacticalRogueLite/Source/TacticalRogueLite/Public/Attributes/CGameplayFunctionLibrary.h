// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CGameplayFunctionLibrary.generated.h"

UCLASS()
class TACTICALROGUELITE_API UCGameplayFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	public:

		UFUNCTION(BlueprintCallable, Category = "Gameplay")
		static bool ApplyDamage(AActor* DamageCauser, AActor* TargetActor, int DamageAmount);

	
		// UFUNCTION(BlueprintPure, Category = "PSO Caching")
		// static int32 GetRemainingBundledPSOs();
	};

