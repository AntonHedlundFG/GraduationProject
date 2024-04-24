// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Debug/CAiDebugWindow.h"

FAiDebugInfo::FAiDebugInfo(ACUnit* Instigator,const ACGridTile* Target, int32 TotalPathsEvaluated,
	const TArray<FActionPath>& ActionPaths)
{
	this->Instigator = (Instigator != nullptr)? Instigator->GetUnitName(): "NULL";
	this->Target = (Target != nullptr)?Target->GetName():"NULL";
	this->TotalPathsEvaluated = TotalPathsEvaluated;
	this->ActionPaths = ActionPaths;
}

FAiDebugInfo::FAiDebugInfo()
{
}

FText UCAiDebugWindow::GetFormattedText_Instigator(const FAiDebugInfo& Package)
{
	return JoinText(FText::FromString("Unit: "),FText::FromString(Package.Instigator));
}

FText UCAiDebugWindow::GetFormattedText_Target(const FAiDebugInfo& Package)
{
	return JoinText(FText::FromString("Target: "),FText::FromString(Package.Target));
}

FText UCAiDebugWindow::GetFormattedText_Paths(const FAiDebugInfo& Package)
{
	return FText::Format(FText::FromString("Paths: {0}/{1}"),FText::AsNumber(CurrentPathIndex+1),FText::AsNumber(Package.ActionPaths.Num()));
}

FText UCAiDebugWindow::GetFormattedText_Package()
{
	return FText::Format(FText::FromString("Package: {0}/{1}"),FText::AsNumber(CurrentPackageIndex+1),FText::AsNumber(Packages.Num()));
}

FText UCAiDebugWindow::GetFormattedText_Evaluation(const FAiDebugInfo& Package)
{
	return FText::Format(FText::FromString("Total Paths Evaluated: {0}"),FText::AsNumber(Package.TotalPathsEvaluated));
}

FText UCAiDebugWindow::GetFormattedText_Score(const FActionPath& Path)
{
	return FText::Format(FText::FromString("Score: {0}"),FText::AsNumber(Path.GetScore()));
}

FText UCAiDebugWindow::GetFormattedText_PathTarget(FActionPath& Path, int Index)
{
	auto PathArr = Path.GetPath();
	if(Index > PathArr.Num() || Index < 0)
	{
		return FText::FromString("Target: IndexOutOfRangeException");
	}
	FString TargetName;
	try
	{
		TargetName = PathArr[Index].Value->GetName();
	}
	catch (...)
	{
		TargetName = "NULL";
	}
	return JoinText(FText::FromString("Target: "),FText::FromString(TargetName));
}

FText UCAiDebugWindow::GetFormattedText_PathAbility(FActionPath& Path, int Index)
{
	auto PathArr = Path.GetPath();
	if(Index > PathArr.Num() || Index < 0)
	{
		return FText::FromString("Target: IndexOutOfRangeException");
	}
	FString AbilityName;
	try
	{
		AbilityName = PathArr[Index].Key.InventorySlotTag.ToString();
	}
	catch (...)
	{
		AbilityName = "NULL";
	}
	return JoinText(FText::FromString("Slot: "),FText::FromString(AbilityName));
}

FText UCAiDebugWindow::JoinText(FText a, FText b)
{
	return FText::Format(FText::FromString(TEXT("{0}{1}")),a,b);
}

void UCAiDebugWindow::UpdateUI()
{
	if(Packages.Num() == 0){return;}
	PackageText->SetText(GetFormattedText_Package());
	InstigatorText->SetText(GetFormattedText_Instigator(Packages[CurrentPackageIndex]));
	EvaluationsText->SetText(GetFormattedText_Evaluation(Packages[CurrentPackageIndex]));
	PathsText->SetText(GetFormattedText_Paths(Packages[CurrentPackageIndex]));
	if(Packages[CurrentPackageIndex].ActionPaths.Num() == 0){return;}
	ScoreText->SetText(GetFormattedText_Score(Packages[CurrentPackageIndex].ActionPaths[CurrentPathIndex]));
	
	for(int i = 0; i < PathContainerAmount; i++)
	{
		PathContainers[i]->SetAbilityText(GetFormattedText_PathAbility(Packages[CurrentPackageIndex].ActionPaths[CurrentPathIndex],i));
		PathContainers[i]->SetTargetText(GetFormattedText_PathTarget(Packages[CurrentPackageIndex].ActionPaths[CurrentPathIndex],i));
	}
}

void UCAiDebugWindow::NativeConstruct()
{
	Super::NativeConstruct();

	for(int i = 0; i < PathContainerAmount; i++)
	{
		UCAiDebugPathContainer* Container = CreateWidget<UCAiDebugPathContainer>(this,PathContainerBlueprint);
		Container->AddToViewport();
		PathVerticalBox->AddChild(Container);
		PathContainers.Add(Container);
	}
	DebugInstance = this;
}

void UCAiDebugWindow::AddPackage(FAiDebugInfo& Package)
{
	Packages.Add(Package);
	CurrentPackageIndex = Packages.Num()-1;
	CurrentPathIndex = 0;
	UpdateUI();
}

void UCAiDebugWindow::StepPackages(StepDirection Direction)
{
	if(Packages.Num() == 0){return;}
	CurrentPathIndex = 0;

	switch(Direction)
	{
	case StepDirection::LEFT:
		CurrentPackageIndex = CurrentPackageIndex-1 < 0?Packages.Num() -1: CurrentPackageIndex-1;
		break;
	case StepDirection::RIGHT:
		CurrentPackageIndex = CurrentPackageIndex+1 >= Packages.Num()? 0: CurrentPackageIndex+1;
		break;
	}
	UpdateUI();
}

void UCAiDebugWindow::StepPaths(StepDirection Direction)
{
	if(Packages.Num() == 0){return;}
	switch(Direction)
	{
	case StepDirection::LEFT:
		CurrentPathIndex = CurrentPathIndex-1 < 0? Packages[CurrentPackageIndex].ActionPaths.Num() -1: CurrentPathIndex-1;
		break;
	case StepDirection::RIGHT:
		CurrentPathIndex = CurrentPathIndex+1 >= Packages[CurrentPackageIndex].ActionPaths.Num()? 0: CurrentPathIndex+1;
		break;
	}
	UpdateUI();
}

UCAiDebugWindow* UCAiDebugWindow::GetInstance()
{
	return DebugInstance;
}
