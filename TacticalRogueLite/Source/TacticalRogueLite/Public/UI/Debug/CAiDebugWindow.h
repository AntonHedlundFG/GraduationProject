// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CAiDebugPathContainer.h"
#include "AI/CAIController.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Grid/CGridTile.h"
#include "GridContent/CUnit.h"
#include "UI/CResizableWindow.h"
#include "CAiDebugWindow.generated.h"

/**
 * 
 */

USTRUCT(Blueprintable)
struct FAiDebugInfo
{
	GENERATED_BODY()
	
	FString Instigator;
	FString Target;
	int32 TotalPathsEvaluated;
	TArray<FActionPath> ActionPaths;
	FAiDebugInfo(ACUnit* Instigator,const ACGridTile* Target,int32 TotalPathsEvaluated,const TArray<FActionPath>& ActionPaths);
	FAiDebugInfo();
};
UENUM(BlueprintType)
enum StepDirection
{
	LEFT   UMETA(DisplayName = "LEFT"),
	RIGHT   UMETA(DisplayName = "RIGHT")
};
UCLASS()
class TACTICALROGUELITE_API UCAiDebugWindow : public UCMovableWindow
{
	GENERATED_BODY()
	
	inline static UCAiDebugWindow* DebugInstance;
	TArray<FAiDebugInfo> Packages;
	int CurrentPackageIndex;
	int CurrentPathIndex;
	const int PathContainerAmount = 4;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PackageText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InstigatorText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* EvaluationsText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PathsText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreText;
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* PathVerticalBox;
	
	UPROPERTY()
	TArray<UCAiDebugPathContainer*> PathContainers;
protected:
	
	FText GetFormattedText_Instigator(const FAiDebugInfo& Package);
	FText GetFormattedText_Target(const FAiDebugInfo& Package);
	FText GetFormattedText_Paths(const FAiDebugInfo& Package);
	FText GetFormattedText_Package();
	FText GetFormattedText_Evaluation(const FAiDebugInfo& Package);
	FText GetFormattedText_Score(const FActionPath& Path);
	FText GetFormattedText_PathTarget(FActionPath& Path,int Index);
	FText GetFormattedText_PathAbility(FActionPath& Path,int Index);
	FText JoinText(FText a, FText b);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<UCAiDebugPathContainer> PathContainerBlueprint;
	void UpdateUI();
	virtual void NativeConstruct() override;
public:
	UFUNCTION(BlueprintCallable)
	void AddPackage(FAiDebugInfo& Package);
	UFUNCTION(BlueprintCallable)
	void StepPackages(StepDirection Direction);
	UFUNCTION(BlueprintCallable)
	void StepPaths(StepDirection Direction);
	static UCAiDebugWindow* GetInstance();
};
