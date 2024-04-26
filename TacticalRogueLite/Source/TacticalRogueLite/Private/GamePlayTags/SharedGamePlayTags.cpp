#include "GamePlayTags/SharedGameplayTags.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Unit, "Unit");
UE_DEFINE_GAMEPLAY_TAG(TAG_Unit_IsPlayer, "Unit.IsPlayer");
UE_DEFINE_GAMEPLAY_TAG(TAG_Unit_IsEnemy, "Unit.IsEnemy");
UE_DEFINE_GAMEPLAY_TAG(TAG_Unit_Killed, "Unit.Killed");

UE_DEFINE_GAMEPLAY_TAG(TAG_Action, "Action");
UE_DEFINE_GAMEPLAY_TAG(TAG_Action_Attacking, "Action.Attacking");
UE_DEFINE_GAMEPLAY_TAG(TAG_Action_CursorSelect, "Action.CursorSelect");
UE_DEFINE_GAMEPLAY_TAG(TAG_Action_Moving, "Action.Moving");
UE_DEFINE_GAMEPLAY_TAG(TAG_Action_DefaultAttack, "Action.DefaultAttack");
UE_DEFINE_GAMEPLAY_TAG(TAG_Action_Rooted, "Action.Rooted");

UE_DEFINE_GAMEPLAY_TAG(TAG_Abilitytype, "AbilityType");
UE_DEFINE_GAMEPLAY_TAG(TAG_Abilitytype_Offensive, "AbilityType.Offensive");
UE_DEFINE_GAMEPLAY_TAG(TAG_Abilitytype_Defensive, "AbilityType.Defensive");
UE_DEFINE_GAMEPLAY_TAG(TAG_Abilitytype_Supporting, "AbilityType.Supporting");


UE_DEFINE_GAMEPLAY_TAG(TAG_Movement, "Movement");
UE_DEFINE_GAMEPLAY_TAG(TAG_Movement_Straight, "Movement.Straight");
UE_DEFINE_GAMEPLAY_TAG(TAG_Movement_Diagonal, "Movement.Diagonal");
UE_DEFINE_GAMEPLAY_TAG(TAG_Movement_Knight, "Movement.Knight");
UE_DEFINE_GAMEPLAY_TAG(TAG_Movement_Flying, "Movement.Flying");

UE_DEFINE_GAMEPLAY_TAG(TAG_Status, "Status");
UE_DEFINE_GAMEPLAY_TAG(TAG_Status_Rooted, "Status.Rooted");
UE_DEFINE_GAMEPLAY_TAG(TAG_Status_Silenced, "Status.Silenced");

UE_DEFINE_GAMEPLAY_TAG(TAG_ItemSlot, "ItemSlot");
UE_DEFINE_GAMEPLAY_TAG(TAG_ItemSlot_Boots, "ItemSlot.Boots");
UE_DEFINE_GAMEPLAY_TAG(TAG_ItemSlot_Weapon, "ItemSlot.Weapon");
UE_DEFINE_GAMEPLAY_TAG(TAG_ItemSlot_Armor, "ItemSlot.Armor");
UE_DEFINE_GAMEPLAY_TAG(TAG_ItemSlot_Trinket, "ItemSlot.Trinket");
UE_DEFINE_GAMEPLAY_TAG(TAG_ItemSlot_Charm, "ItemSlot.Charm");


//UE_DEFINE_GAMEPLAY_TAG(Status_Rooted, "ItemSlot.Boots");

UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute, "Attribute");
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_AttackDamage, "Attribute.AttackDamage");
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_CritChance, "Attribute.CritChance");
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_Armor, "Attribute.Armor");
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_Health, "Attribute.Health");
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_HealthMax, "Attribute.HealthMax");
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_ShieldPower, "Attribute.ShieldPower");

// UE_DEFINE_GAMEPLAY_COMMENT() https://www.unrealdirective.com/tip/declare-define-native-gameplay-tags
