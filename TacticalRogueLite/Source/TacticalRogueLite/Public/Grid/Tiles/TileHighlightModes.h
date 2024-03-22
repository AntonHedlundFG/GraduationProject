
#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ETileHighlightModes : uint8
{
	ETHM_NONE			UMETA(DisplayName = "None"),
	ETHM_Default		UMETA(DisplayName = "No Highlight"),
	ETHM_Hovered		UMETA(DisplayName = "Hovered"),
	ETHM_Attackable		UMETA(DisplayName = "Attackable"),
	ETHM_Reachable		UMETA(DisplayName = "Reachable"),
	ETHM_PartOfPath		UMETA(DisplayName = "Part of Path"),
	ETHM_MAX			UMETA(Hidden),
};