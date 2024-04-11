#include "AI/CConsideration.h"


float UCConsideration::Evaluate_Implementation(FAbility& Ability, ACGridTile* StartTile, ACGridTile* TargetTile, FCAIContext& Context)
{
	LOG_WARNING("Consideration %s has no implementation for Evaluate", *Name);
	return 0;
}