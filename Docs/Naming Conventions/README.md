## Classes

- ### UObject

    - C++ (Optional): UCExampleClass
    - AS (Optional): USExampleClass
    - Blueprint (BP) Parent (Optional): BP_ExampleClassBase
    - BP Child: BP_ExampleClass

- ### AActor

    - C++ (Optional): ACExampleActor
    - AS (Optional): ASExampleActor
    - BP Parent (Optional): BP_ExampleActorBase
    - BP Child: BP_ExampleActor

- ### C++

```cpp 
class TACTICALROGUELITE_API ACExamplePawn : public APawn
{
};
```

- ### AS

```c# 
class ASExamplePawn : ACExamplePawn
{
};
```

- ### Blueprints

- Parent class: `BP_ExamplePawnBase`
- Child Classes: `BP_HeroPawn`, `BP_EnemyPawn`

# Struct:

- ### C++

```cpp 
struct FExampleStruct 
{
}
```

- ### AS

```c# 
struct FExampleStruct
{
}
```

# Enum:

- ### C++

```cpp 
enum class EDirections : uint8
{
    ED_NONE             UMETA(Hidden), // Can have ("Displayname = "None") instead of (Hidden) if None should be a selectable option
    ED_North            UMETA(Displayname = "North"),
    ED_South            UMETA(Displayname = "South"),
    // ...
    MAX                 UMETA(Hidden)
}
```

- ### AS

```c#
enum EDirections
{
    ED_NONE,
    ED_North,
    ED_South,
    // ...
    ED_MAX
}
```

# Properties:

```c#
class USFieldExample : AActor
{
    // If a class replicates, define it at the top of the class for easy reference.
    default bReplicates = true;

    UPROPERTY(EditAnywhere)
    bool bExampleBool;

    UPROPERTY(EditAnywhere, Category = "First Category")
    int ExampleInt;

    UPROPERTY(EditDefaultsOnly, Category = "Second Category")
    char ExampleChar;

    UPROPERTY(VisibleAnywhere,  Category = "First Category|Child to Second Category")
    float ExampleFloat;

    UPROPERTY(VisibleAnywhere,  Category = "First Category|Child to Second Category", meta = (EditCondition = "bExampleFloat"))
    float ExampleFloat2;

    UPROPERTY(VisibleAnywhere,  Category = "First Category|Child to Second Category", meta = (ClampMin = 0.0f, ClampMax = 100.0f))
    float ExampleFloat3;

    UPROPERTY(NotVisible)
    double ExampleDouble;

    UPROPERTY(NotEditable)
    AActor ExampleActor;

    // Etc...
}
```

# Params:

- ### C++

```cpp
UFUNCTION()
void UCExampleClass::FunctionParamExample(FStruct inStruct)
{

}

UFUNCTION()
void UCExampleClass::FunctionParamExample(FStruct& outStruct)
{

} 
```

- ### AS

```c#
UFUNCTION()
void FunctionParamExample(FStruct inStruct)
{

}

UFUNCTION()
void FunctionParamExample(FStruct& outStruct)
{

}
```
