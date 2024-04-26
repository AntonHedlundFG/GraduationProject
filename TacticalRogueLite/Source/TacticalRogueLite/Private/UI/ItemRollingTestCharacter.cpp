// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ItemRollingTestCharacter.h"

#include "Engine/AssetManager.h"
#include "GamePlay/Subsystems/CItemRollingSubSystem.h"
#include "ItemData/CItemData.h"
#include "Utility/Logging/CLogManager.h"

// Sets default values
AItemRollingTestCharacter::AItemRollingTestCharacter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AItemRollingTestCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemRollingTestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemRollingTestCharacter::DebugGetItemFromRoll()
{
	UCItemRollingSubSystem* RollingSubSystem = GetWorld()->GetGameInstance()->GetSubsystem<UCItemRollingSubSystem>();
	TArray<FItemRollResult> RollResults;
	TArray<FPrimaryAssetId> ExcludeIDs;
	FGameplayTagContainer RequireTags;
	RequireTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Class.Monk")));
	if(Buckets.IsEmpty()){Buckets.Add(FBucketInfo());}
	RollingSubSystem->RollItemTable(LootTable,RollResults,RequireTags,ExcludeIDs,Buckets,6,ERollType::WithoutReplacement);
	
		for(auto Result : RollResults)
		{
			UCItemData* Data = RollingSubSystem->GetItem(Result.ItemID);
			if(Data)
			{
				LOG(ELogCategory::LC_Gameplay,"Rolled Item: %s",*Data->GetItemName().ToString());	
			}
		}	
}

void AItemRollingTestCharacter::OnItemLoaded(TArray<FPrimaryAssetId> LoadedAssets)
{
	
}

