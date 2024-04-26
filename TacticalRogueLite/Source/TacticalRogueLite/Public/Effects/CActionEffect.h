
#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction.h"
#include "Utility/TurnTimer/CTurnTimerSubsystem.h"
#include "CActionEffect.generated.h"

class ACUnit;

/* TODO: This should be an effect that uses a timer. When the timer is done
* OnTimerFinished should be called.
*/
UCLASS()
class TACTICALROGUELITE_API UCActionEffect : public UCAction
{
	GENERATED_BODY()

public:
	UCActionEffect();
	
	virtual void StartAction(AActor* Instigator) override;

	virtual void StopAction(AActor* Instigator) override;
	
protected:
	//TODO: Time not relevant in this project. Turn into 'turns'. 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	int DurationInTurns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	EProgressTurnMethod ProgressTurnMethod;

	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void OnTimerFinished(AActor* AffectedUnit);

	
};
