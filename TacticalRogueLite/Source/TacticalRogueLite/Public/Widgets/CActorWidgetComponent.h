
#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "CActorWidgetComponent.generated.h"


class UCActorWidget;

/**
 * Extension of the WidgetComponent to make it easy to have owning Actor context to the Widget. Commonly used to display health bars, names, and interaction panels above Actors.
	Automatically calls SetOwningActor on the widget if the correct type of widget is used (ActorAttachWidget).
 */

UCLASS(meta = (BlueprintSpawnableComponent))
class TACTICALROGUELITE_API UCActorWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:

	virtual void InitWidget() override;
	
	UCActorWidgetComponent();
	
};
