#include "GamePlayTags/SharedGameplayTags.h"

namespace SharedGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(Level_IsCurrentLevel, "Level.IsCurrentLevel"); //Experimental, might be good use for levelstreaming....
	
	UE_DEFINE_GAMEPLAY_TAG(Unit, "Unit");
	UE_DEFINE_GAMEPLAY_TAG(Unit_IsPlayer, "Unit.IsPlayer");
	UE_DEFINE_GAMEPLAY_TAG(Unit_IsEnemy, "Unit.IsEnemy");
	
	UE_DEFINE_GAMEPLAY_TAG(Action, "Action");
	UE_DEFINE_GAMEPLAY_TAG(Action_Attacking, "Action.Attacking");
	UE_DEFINE_GAMEPLAY_TAG(Action_CursorSelect, "Action.CursorSelect");
	UE_DEFINE_GAMEPLAY_TAG(Action_Moving, "Action.Moving");
	UE_DEFINE_GAMEPLAY_TAG(Action_DefaultAttack, "Action.DefaultAttack");
	UE_DEFINE_GAMEPLAY_TAG(Action_Rooted, "Action.Rooted");

	UE_DEFINE_GAMEPLAY_TAG(Movement, "Movement");
	UE_DEFINE_GAMEPLAY_TAG(Movement_Straight, "Movement.Straight");
	UE_DEFINE_GAMEPLAY_TAG(Movement_Diagonal, "Movement.Diagonal");
	UE_DEFINE_GAMEPLAY_TAG(Movement_Knight, "Movement.Knight");
	UE_DEFINE_GAMEPLAY_TAG(Movement_Flying, "Movement.Flying");


	UE_DEFINE_GAMEPLAY_TAG(Status_Rooted, "Status.Rooted");

	UE_DEFINE_GAMEPLAY_TAG(ItemSlot, "ItemSlot");
	UE_DEFINE_GAMEPLAY_TAG(ItemSlot_Boots, "ItemSlot.Boots");
	UE_DEFINE_GAMEPLAY_TAG(ItemSlot_Weapon, "ItemSlot.Weapon")
	UE_DEFINE_GAMEPLAY_TAG(ItemSlot_Armor, "ItemSlot.Armor");
	UE_DEFINE_GAMEPLAY_TAG(ItemSlot_Helmet, "ItemSlot.Helmet");
	UE_DEFINE_GAMEPLAY_TAG(ItemSlot_Ring, "ItemSlot.Ring");;
	UE_DEFINE_GAMEPLAY_TAG(ItemSlot_Charm, "ItemSlot.Charm");
	

	//UE_DEFINE_GAMEPLAY_TAG(Status_Rooted, "ItemSlot.Boots");

	UE_DEFINE_GAMEPLAY_TAG(Attribute, "Attribute");
	UE_DEFINE_GAMEPLAY_TAG(Attribute_AttackDamage, "Attribute.AttackDamage");
	UE_DEFINE_GAMEPLAY_TAG(Attribute_CritChance, "Attribute.CritChance");
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Armor, "Attribute.Armor");
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Health, "Attribute.Health");
	UE_DEFINE_GAMEPLAY_TAG(Attribute_ShieldPower, "Attribute.ShieldPower");

	// UE_DEFINE_GAMEPLAY_TAG_COMMENT() https://www.unrealdirective.com/tip/declare-define-native-gameplay-tags
}