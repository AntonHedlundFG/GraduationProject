#include "GamePlayTags/SharedGameplayTags.h"

namespace SharedGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(Action_MoveTo, "Action.MoveTo");
	UE_DEFINE_GAMEPLAY_TAG(Action_DefaultAttack, "Action.DefaultAttack");
	UE_DEFINE_GAMEPLAY_TAG(Action_Rooted, "Action.Rooted");

	UE_DEFINE_GAMEPLAY_TAG(Status_Rooted, "Status.Rooted");

	// UE_DEFINE_GAMEPLAY_TAG_COMMENT() https://www.unrealdirective.com/tip/declare-define-native-gameplay-tags
}