
#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction.h"
#include "Utility/TurnTimer/CTurnTimerSubsystem.h"
#include "CActionEffect.generated.h"

class ACUnit;

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
	int DurationInTurns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	EProgressTurnMethod ProgressTurnMethod;

	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void ExecuteEffect(AActor* AffectedUnit);
	
};
