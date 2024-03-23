#pragma once

#include "NativeGameplayTags.h"

namespace SharedGameplayTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Level_IsCurrentLevel);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Pawn_IsPlayer);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Action_Attacking);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Action_CursorSelect);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Action_MoveTo);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Action_DefaultAttack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Action_Rooted);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Rooted);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_AttackDamage);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_CritChance);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Armor);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Health);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_ShieldPower);
	
}