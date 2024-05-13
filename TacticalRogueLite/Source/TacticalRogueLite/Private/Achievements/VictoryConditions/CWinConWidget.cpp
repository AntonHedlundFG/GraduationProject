

#include "Achievements/VictoryConditions/CWinConWidget.h"

void UCWinConWidget::UpdateWinConText(FString inText)
{
	if (inText.IsEmpty())
		return;

	CurrentText = inText;
}
