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
	
	//Fetch all achievements through Asset Manager.
	if (UAssetManager* Manager = UAssetManager::GetIfInitialized())
	{
		TArray<FPrimaryAssetId> AssetList;
		const FPrimaryAssetType AssetType("Achievement");
		
		Manager->GetPrimaryAssetIdList(AssetType, AssetList);

		TArray<FName> Bundles;
		Bundles.Add("Actions");
		
		//Different syntax, creating a delegate via CreateUObject and passing
		//in the parameters we want to use once loading has completed several frames or seconds later.
		//In this case the AssetList. 
		FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &AItemRollingTestCharacter::OnItemLoaded, AssetList);

		//Requests the load in Asset Manager on the AssetList and passes in the Delegate we just created.
		Manager->LoadPrimaryAssets(AssetList, Bundles, Delegate);
	}
	
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
	TArray<FBucketInfo> Bucket;
	FGameplayTagContainer RequireTags;
	RequireTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Class.Monk")));
	Bucket.Add(FBucketInfo());
	RollingSubSystem->RollItemTable(LootTable,RollResults,RequireTags,ExcludeIDs,Bucket,5,ERollType::WithoutReplacement);

	auto Manager = UAssetManager::GetIfInitialized();
	if(Manager)
	{
		for(auto Result : RollResults)
		{
			UCItemData* Data = Cast<UCItemData>(Manager->GetPrimaryAssetObject(Result.ItemID));
			if(Data)
			{
				LOG(ELogCategory::LC_Info,"Rolled Item: %s",*Data->GetItemName().ToString());	
			}
		}	
	}
}

void AItemRollingTestCharacter::OnItemLoaded(TArray<FPrimaryAssetId> LoadedAssets)
{
}

