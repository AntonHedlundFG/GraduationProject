#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MathExtendedLibrary.generated.h"

struct FMathExtendedLibrary : FMath {

	template<typename T>
	UE_NODISCARD static constexpr FORCEINLINE T EaseInOutBack(T start, T end, T alpha)
	{
		const T c1 = 1.70158;
		const T c2 = c1 * 1.525;
		T easedValue;

		if (alpha < 0.5) {
			easedValue = (pow(2 * alpha, 2) * ((c2 + 1) * 2 * alpha - c2)) / 2;
		} else {
			easedValue = (pow(2 * alpha - 2, 2) * ((c2 + 1) * (2 * alpha - 2) + c2) + 2) / 2;
		}

		return start + (end - start) * easedValue;
	}

	UE_NODISCARD static FORCEINLINE FVector EaseInOutBack(const FVector& start, const FVector& end, const float alpha) {
		constexpr float c1 = 1.70158f;
		constexpr float c2 = c1 * 1.525f;

		auto easeInOutBackComponent = [c2](float start, float end, float alpha) -> float {
			float x = alpha;
			float easedValue;
			if (x < 0.5f) {
				easedValue = Pow(2 * x, 2) * ((c2 + 1) * 2 * x - c2) / 2;
			} else {
				easedValue = (Pow(2 * x - 2, 2) * ((c2 + 1) * (2 * x - 2) + c2) + 2) / 2;
			}
			return start + (end - start) * easedValue;
		};

		float easedX = easeInOutBackComponent(start.X, end.X, alpha);
		float easedY = easeInOutBackComponent(start.Y, end.Y, alpha);
		float easedZ = easeInOutBackComponent(start.Z, end.Z, alpha);

		return FVector(easedX, easedY, easedZ);
	}
	
};

UCLASS()
class TACTICALROGUELITE_API UMathExtendedLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Math|Extended", meta = (DisplayName = "EaseInOutBack (float)"))
	static float EaseInOutBackFloat(float start, float end, float alpha) {
		return FMathExtendedLibrary::EaseInOutBack(start, end, alpha);
	}

	UFUNCTION(BlueprintCallable, Category = "Math|Extended", meta = (DisplayName = "EaseInOutBack (double)"))
	static double EaseInOutBackDouble(double start, double end, double alpha) {
		return FMathExtendedLibrary::EaseInOutBack(start, end, alpha);
	}

	UFUNCTION(BlueprintCallable, Category = "Math|Extended", meta = (DisplayName = "EaseInOutBack (vector)"))
	static FVector EaseInOutBackVector(FVector start, FVector end, float alpha) {
		return FMathExtendedLibrary::EaseInOutBack(start, end, alpha);
	}

};