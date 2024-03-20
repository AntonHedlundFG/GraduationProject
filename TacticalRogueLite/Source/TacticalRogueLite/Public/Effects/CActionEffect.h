
#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction.h"
#include "CActionEffect.generated.h"


UCLASS()
class TACTICALROGUELITE_API UCActionEffect : public UCAction
{
	GENERATED_BODY()

public:
	UCActionEffect();
	
	virtual void StartAction_Implementation(AActor* Instigator) override;

	virtual void StopAction_Implementation(AActor* Instigator) override;
	
protected:
	//TODO: Time not relevant in this project. Turn into 'turns'. 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Duration;

	//Time between 'ticks' to apply. Not relevant. 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Period;

	FTimerHandle PeriodHandle;
	FTimerHandle DurationHandle;

	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void ExecuteEffect(AActor* Instigator);

public:

	UFUNCTION(BlueprintCallable, Category = "Action")
	float GetTimeRemaining() const;
	
};
