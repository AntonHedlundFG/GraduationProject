#include "Grid/CTileHighlightComponent.h"

void UCHighlightComponent::Init(UPrimitiveComponent* inPrimitiveComponent)
{
	HighlightedComponent = inPrimitiveComponent;
	AppendHighlightMode(ETileHighlightModes::ETHM_Default);
}

void UCHighlightComponent::AppendHighlightMode(ETileHighlightModes inHighlightMode)
{
	HighlightModes.Add(inHighlightMode);
	HighlightModes.Sort();
	UpdateHighlight();
}

void UCHighlightComponent::RemoveHighlightMode(ETileHighlightModes inHighlightMode)
{
	HighlightModes.Remove(inHighlightMode);
	if(HighlightModes.Num() == 0)
	{
		HighlightModes.Add(ETileHighlightModes::ETHM_Default);
	}
	UpdateHighlight();
}

void UCHighlightComponent::UpdateHighlight()
{
	constexpr uint8 Max =  static_cast<uint8>(ETileHighlightModes::ETHM_MAX);
	uint8 HighlightValue = 256 - Max; // 256 is the max value for custom depth stencil
	HighlightValue +=  static_cast<uint8>(HighlightModes.Top());
	if (HighlightedComponent)
	{
		HighlightedComponent->CustomDepthStencilValue = HighlightValue;
		HighlightedComponent->MarkRenderStateDirty();
	}
}
