// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CAbilityWidget.generated.h"

class UCAttributeComponent;
class USizeBox;

UCLASS()
class TACTICALROGUELITE_API UCAbilityWidget : public UUserWidget
{
	GENERATED_BODY()


protected:

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> ParentSizeBox;
	
	UPROPERTY() //?
	TObjectPtr<UCAttributeComponent> AttributeComp;

public:

	UPROPERTY(EditAnywhere, Category = "UI")
	FVector WorldOffset;
	
	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (ExposeOnSpawn=true))
	TObjectPtr<AActor> AttachedActor;
};
