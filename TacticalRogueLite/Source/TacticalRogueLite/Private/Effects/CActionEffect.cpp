//TODO: Time not relevant in this project. Turn into 'turns'.
//TODO: Impl gameplaycues to handle visuals.

#include "Effects/CActionEffect.h"
#include "Actions/CActionComponent.h"
#include "GridContent/CUnit.h"
#include "GameFramework/GameStateBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CActionEffect)

UCActionEffect::UCActionEffect()
{

}

void UCActionEffect::StartAction(AActor* Instigator)
{
	Super::StartAction(Instigator);

	if (DurationInTurns > 0)
	{
		//auto* TimerSystem = GetWorld()->GetSubsystem<UCTurnTimerSubsystem>();
		//TimerSystem->SetTimer(DurationInTurns, )
	}

}

void UCActionEffect::StopAction(AActor* Instigator)
{

	Super::StopAction(Instigator);

}


void UCActionEffect::OnTimerFinished_Implementation(AActor* AffectedUnit)
{

}