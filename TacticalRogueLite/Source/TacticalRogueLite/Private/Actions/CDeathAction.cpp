#include "Actions/CDeathAction.h"
#include "CGameState.h"
#include "GridContent/CUnit.h"
#include "Net/UnrealNetwork.h"
#include "Actions/CActionComponent.h"
#include "Utility/Logging/CLogManager.h"

void UCDeathAction::StartAction(AActor* Instigator)
{
	Super::StartAction(Instigator);

	if (!AffectedUnit)
	{
		LOG_WARNING("AffectedUnit has not been set, cannot kill nullptrs");
		return;
	}

	ACGameState* GameState = AffectedUnit->GetWorld()->GetGameState<ACGameState>();
	if (!GameState)
	{
		LOG_WARNING("No GameState reference available");
		return;
	}

	

	DeathTurnOrderIndex = GameState->RemoveUnitFromOrder(AffectedUnit);
	if (DeathTurnOrderIndex == -2)
	{
		LOG_WARNING("Unit was not in turn order");
	}

	DeathTile = AffectedUnit->GetTile();

	AffectedUnit->SetTile(nullptr);
	AffectedUnit->GetActionComp()->ActiveGameplayTags.AppendTag(FGameplayTag::RequestGameplayTag("Unit.Killed"));
	
	LOG_GAMEPLAY("%s died.", *AffectedUnit->GetUnitName());

}

void UCDeathAction::UndoAction(AActor* Instigator)
{

	if (!AffectedUnit)
	{
		LOG_WARNING("Lacking unit reference for undoing death");
		return;
	}
	
	ACGameState* GameState = AffectedUnit->GetWorld()->GetGameState<ACGameState>();
	if (!GameState)
	{
		LOG_WARNING("Lacking gamestate reference for undoing death");
		return;
	}
	AffectedUnit->GetActionComp()->ActiveGameplayTags.RemoveTag(FGameplayTag::RequestGameplayTag("Unit.Killed"));
	AffectedUnit->SetTile(DeathTile);
	if (DeathTurnOrderIndex > -2)
		GameState->AddUnitToOrder(AffectedUnit, DeathTurnOrderIndex);
	else
		LOG_WARNING("Trying to resurrect unit that wasn't in queue");

	LOG_GAMEPLAY("%s resurrected.", *AffectedUnit->GetUnitName());

	Super::UndoAction(Instigator);

}

void UCDeathAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCDeathAction, AffectedUnit);
}