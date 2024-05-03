
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Containers/ArrayView.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "CAttributeUtilities.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStackChanged);

/**
 *	===================== Fast TArray Replication ===================== 
 *
 *	Fast TArray Replication is a custom implementation of NetDeltaSerialize that is suitable for TArrays of UStructs. It offers performance
 *	improvements for large data sets, it serializes removals from anywhere in the array optimally, and allows events to be called on clients
 *	for adds and removals. The downside is that you will need to have game code mark items in the array as dirty, and well as the *order* of the list
 *	is not guaranteed to be identical between client and server in all cases. https://zhuanlan.zhihu.com/p/537613327 
 *
 */

USTRUCT(BlueprintType)
struct FGameplayTagStack : public FFastArraySerializerItem
{
    GENERATED_USTRUCT_BODY()
 
    FGameplayTagStack()
    {}
 
    FGameplayTagStack(FGameplayTag InTag, int32 InCount)
        : Tag(InTag)
    {
        Tag = InTag;
        Count = InCount;
    }

    private:

    friend FGameplayTagStackContainer;
 
    UPROPERTY()
    FGameplayTag Tag;
 
    UPROPERTY()
    int32 Count = 0;
 
};
 
 
USTRUCT(BlueprintType)
struct FGameplayTagStackContainer : public FFastArraySerializer
{
    GENERATED_USTRUCT_BODY()
 
    FGameplayTagStackContainer() { }
 
public:
 
    void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
    void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
    void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
     
    bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
    {
        return FastArrayDeltaSerialize<FGameplayTagStack, FGameplayTagStackContainer>(Stacks, DeltaParms, *this);
    }

    /* NOT FULLY TESTED
     * Empties the container
     */
    void ClearAllStacks();
 
    /* 
     * Adds count to the tag's stack.
     */
    void AddStackCount(FGameplayTag Tag, int32 Count);
 
    /*
     * Removes count from the tag's stack.
     */
    void RemoveStackCount(FGameplayTag Tag, int32 Count);

    /*
     * Adds 1 count of this tag
     */
    void AppendTag(FGameplayTag Tag);

    /*
    * Adds 1 count of each tag in this container
    */
    void AppendTags(FGameplayTagContainer Tags);

    /*
     * Removes 1 count of this tag
     */
    void RemoveTag(FGameplayTag Tag);

    /*
    * Removes 1 count of each tag in this container
    */
    void RemoveTags(FGameplayTagContainer Tags);
     
    /* 
     * Returns true if we have a one or more of the tag in the stack.
     */
    bool HasTag(FGameplayTag Tag) const
    {
        return TagCountMap.Contains(Tag);
    }

	bool HasAny(FGameplayTagContainer Tags);

	FGameplayTagContainer GetContainerWithoutStacks();
	
    /*
     * Returns the amount of the stack we have for the supplied tag.
     */
    int32 GetStackCount(FGameplayTag Tag) const
    {
        return TagCountMap.FindRef(Tag); 
    }

    UPROPERTY()
    FOnStackChanged OnStackChanged;

     
 
private:
    // List of gameplay tag stacks. Use the accelerated TagCountMap for checking tag count, etc.
    UPROPERTY()
    TArray<FGameplayTagStack> Stacks;
     
    // Accelerated list for queries
    TMap<FGameplayTag, int32> TagCountMap;
};
 
template<>
struct TStructOpsTypeTraits<FGameplayTagStackContainer> : public TStructOpsTypeTraitsBase2<FGameplayTagStackContainer>
{
    enum
    {
        WithNetDeltaSerializer = true,
    };
};

