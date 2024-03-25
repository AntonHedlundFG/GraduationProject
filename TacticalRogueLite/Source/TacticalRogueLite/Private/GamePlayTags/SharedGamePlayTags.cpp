#include "GamePlayTags/SharedGameplayTags.h"

namespace SharedGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(Level_IsCurrentLevel, "Level.IsCurrentLevel"); //Experimental, might be good use for levelstreaming....
	
	UE_DEFINE_GAMEPLAY_TAG(Action_IsPlayer, "Pawn.IsPlayer");
	
	UE_DEFINE_GAMEPLAY_TAG(Action_Attacking, "Action.Attacking");
	UE_DEFINE_GAMEPLAY_TAG(Action_CursorSelect, "Action.CursorSelect");
	UE_DEFINE_GAMEPLAY_TAG(Action_MoveTo, "Action.MoveTo");
	UE_DEFINE_GAMEPLAY_TAG(Action_DefaultAttack, "Action.DefaultAttack");
	UE_DEFINE_GAMEPLAY_TAG(Action_Rooted, "Action.Rooted");

	UE_DEFINE_GAMEPLAY_TAG(Status_Rooted, "Status.Rooted");

	UE_DEFINE_GAMEPLAY_TAG(Attribute_AttackDamage, "Attribute.AttackDamage");
	UE_DEFINE_GAMEPLAY_TAG(Attribute_CritChance, "Attribute.CritChance");
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Armor, "Attribute.Armor");
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Health, "Attribute.Health");
	UE_DEFINE_GAMEPLAY_TAG(Attribute_ShieldPower, "Attribute.ShieldPower");

	// UE_DEFINE_GAMEPLAY_TAG_COMMENT() https://www.unrealdirective.com/tip/declare-define-native-gameplay-tags
}