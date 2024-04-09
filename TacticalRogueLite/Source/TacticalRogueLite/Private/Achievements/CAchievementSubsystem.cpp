// Fill out your copyright notice in the Description page of Project Settings.


#include "Achievements/CAchievementSubsystem.h"
#include "Actions/CActionComponent.h"
#include "Engine/AssetManager.h"
#include "Actions/CAction.h"
#include "CGameState.h"
#include "CGameInstance.h"
#include "ItemData/CData_Achievement.h"
#include "Utility/SaveGame/CSaveGame.h"
//#include "Game/CItemData_Achievement.h"


void UCAchievementSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

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
		FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &UCAchievementSubsystem::OnAchievementsLoaded, AssetList);

		//Requests the load in Asset Manager on the AssetList and passes in the Delegate we just created.
		Manager->LoadPrimaryAssets(AssetList, Bundles, Delegate);
	}
}

void UCAchievementSubsystem::OnAchievementsLoaded(TArray<FPrimaryAssetId> LoadedAssets)
{
	//Start actions that will listen for achievements.
	if (UAssetManager* Manager = UAssetManager::GetIfInitialized())
	{
		if (ACGameState* GS = GetWorld()->GetGameState<ACGameState>())
		{
			for (FPrimaryAssetId Id : LoadedAssets)
			{
				UCData_Achievement* Item = Cast<UCData_Achievement>(Manager->GetPrimaryAssetObject(Id));
				if (Item == nullptr)
				{
					continue;
				}
				//Make sure to not start achievements that aren´t set up properly.
				if (Item->OwnedTags.Num() == 0)
				{
					LOG_WARNING("Achievement have no owning tags!");
					continue;
				}

				//Skip if achievement is already unlocked.
				if (IsAchievementUnlocked(Item->OwnedTags))
				{
					continue;
				}

				//Start all relevant "listeners" that will check required conditions for achievements to unlock.
				for (TSoftClassPtr<UCAction> SoftClass : Item->ActionClasses)
				{
					//Soft-refs already loaded by AssetManager.
					TSubclassOf<UCAction> ActionClass = SoftClass.Get();
					//UCAction* NewAction = GS->GetActionComp()->AddAction(ActionClass); Maybe we can do this?:>

					//Sanity check that tags are empty, we expect the UCData to fill it below instead to easily keep them in sync for unlocking.
					//ensureAlways(NewAction->ActivationTag.Num() == 0);

					//NewAction->ActivationTag.AppendTags(Item->OwnedTags);
					
				}
			}
		}
	}
}

void UCAchievementSubsystem::UnlockAchievement(FGameplayTagContainer InTags)
{
	UCGameInstance* GI = Cast<UCGameInstance>(GetGameInstance());
	if (ensure(GI))
	{
		//UCSaveGame* SG = GI->GetSaveGame();
		//SG->UnlockedAchievements.AppendTags(InTags);

		//UI/Blueprint will figure out which Data this belongs to.
		OnAchievementUnlocked.Broadcast(InTags);

		//Saves all except the "current run" data.
		//GI->SavePersistentData();

		LOG_WARNING("Unlocked Achievement with tag", *InTags.ToStringSimple());
	}
}

bool UCAchievementSubsystem::IsAchievementUnlocked(FGameplayTagContainer InTags)
{
	//Skip if achievement is already unlocked.
	UCGameInstance* GI = Cast<UCGameInstance>(GetGameInstance());
	if(ensure(GI))
	{
		//UCSaveGame SG = GI->GetSaveGame();
		//if (SaveGame->UnlockedAchievements.HasAll(InTags))
		{
			return true;
		}
	}
	return false;
}

