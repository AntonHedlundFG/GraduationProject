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
enum class EExampleEnum : uint8
{
    EE_NONE         	UMETA("Displayname = "None"),
    EE_EnumOne          UMETA("Displayname = "Enum One"),
    EE_EnumTwo          UMETA("Displayname = "Enum Two"),
    // ...
    EE_MAX              UMETA("Displayname = "Max")
}
```

- ### AS

```c#
enum EExampleEnum
{
    EE_NONE,
    EE_EnumOne,
    EE_EnumTwo,
    // ...
    EE_MAX
}
```

# Properties:

```c#
class USFieldExample : UObject
{
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
