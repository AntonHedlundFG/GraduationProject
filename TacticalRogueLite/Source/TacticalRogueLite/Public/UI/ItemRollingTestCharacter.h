// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemRollingTestCharacter.generated.h"

UCLASS()
class TACTICALROGUELITE_API AItemRollingTestCharacter : public AActor
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	UDataTable* LootTable;
public:	
	// Sets default values for this actor's properties
	AItemRollingTestCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void DebugGetItemFromRoll();
	UFUNCTION()
	void OnItemLoaded(TArray<FPrimaryAssetId> LoadedAssets);
};
