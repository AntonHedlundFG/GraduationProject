//TODO: Time not relevant in this project. Turn into 'turns'.
//TODO: Impl gameplaycues to handle visuals.

#include "Effects/CActionEffect.h"
#include "Actions/CActionComponent.h"
#include "GridContent/CUnit.h"
#include "GameFramework/GameStateBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CActionEffect)

UCActionEffect::UCActionEffect()
{
	bAutoStart = true;
}

void UCActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (DurationInTurns > 0)
	{
		//auto* TimerSystem = GetWorld()->GetSubsystem<UCTurnTimerSubsystem>();
		//TimerSystem->SetTimer(DurationInTurns, )
	}

}

void UCActionEffect::StopAction_Implementation(AActor* Instigator)
{

	Super::StopAction_Implementation(Instigator);

	UCActionComponent* Comp = GetOwningComponent();
	Comp->RemoveAction(this);
}


void UCActionEffect::ExecuteEffect_Implementation(AActor* AffectedUnit)
{

}