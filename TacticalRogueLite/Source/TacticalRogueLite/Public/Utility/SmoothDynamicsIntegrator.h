#pragma once

#include "CoreMinimal.h"
#include "SmoothDynamicsIntegrator.generated.h"

UCLASS(BlueprintType)
class USmoothDynamicsIntegrator : public UActorComponent
{
	GENERATED_BODY()
public:
	USmoothDynamicsIntegrator();
	UFUNCTION(BlueprintCallable)
	void Initialize(const FVector& InitialPosition);
	UFUNCTION(BlueprintCallable)
	FVector Update(float DeltaTime, const FVector& TargetPosition, FVector Velocity = FVector::ZeroVector);
	
	UPROPERTY(EditAnywhere)
	float ResponseFrequency;
	UPROPERTY(EditAnywhere)
	float ResponseDamping;
	UPROPERTY(EditAnywhere)
	float ResponseUnderShoot;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float TMax;

private:
	FVector Previous;
	FVector Current;
	FVector Delta;
	float K1, K2, K3;

	FVector X0;
};
