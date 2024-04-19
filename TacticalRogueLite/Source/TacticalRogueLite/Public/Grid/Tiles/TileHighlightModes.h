
#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ETileHighlightModes : uint8
{
	// Sort by priority from lowest to highest.
	// The highest will be the one displayed if there are multiple
	ETHM_NONE			UMETA(DisplayName = "None"),
	ETHM_Default		UMETA(DisplayName = "No Highlight"),
	ETHM_Reachable		UMETA(DisplayName = "Reachable"),
	ETHM_Attackable		UMETA(DisplayName = "Attackable"),
	ETHM_PartOfPath		UMETA(DisplayName = "Part of Path"),
	ETHM_Hovered		UMETA(DisplayName = "Hovered"),
	ETHM_MAX			UMETA(Hidden),
};