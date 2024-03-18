
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGridContent.generated.h"

UCLASS()
class TACTICALROGUELITE_API ACGridContent : public AActor
{
	GENERATED_BODY()
	
public:	
	ACGridContent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
