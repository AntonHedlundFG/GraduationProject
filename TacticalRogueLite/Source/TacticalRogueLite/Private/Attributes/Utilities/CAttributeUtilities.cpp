// Fill out your copyright notice in the Description page of Project Settings.

#include "Attributes/Utilities/CAttributeUtilities.h"
#include "Containers/ArrayView.h"

/**
*		-Declare a UPROPERTY of your FExampleArray (step 2) type.
*		-You MUST call MarkItemDirty on the FExampleArray when you change an item in the array. You pass in a reference to the item you dirtied. 
*			See FFastArraySerializer::MarkItemDirty.
*		-You MUST call MarkArrayDirty on the FExampleArray if you remove something from the array.
*		-In your classes GetLifetimeReplicatedProps, use DOREPLIFETIME(YourClass, YourArrayStructPropertyName);
*
**/

void FGameplayTagStackContainer::ClearAllStacks()
{
    Stacks.Empty();
    TagCountMap.Empty();
    MarkArrayDirty();
    OnStackChanged.Broadcast();
}

void FGameplayTagStackContainer::AddStackCount(FGameplayTag Tag, int32 Count)
{
    if (Count > 0)
    {
        for (FGameplayTagStack& Stack : Stacks)
        {
            if (Stack.Tag == Tag)
            {
                const int32 NewCount = Stack.Count + Count;
                Stack.Count = NewCount;
                TagCountMap[Tag] = NewCount;
                MarkItemDirty(Stack);
                OnStackChanged.Broadcast();
                return;
            }
        }
 
        FGameplayTagStack& NewStack = Stacks.Emplace_GetRef(Tag, Count);
        MarkItemDirty(NewStack);
        TagCountMap.Add(Tag, Count);
        OnStackChanged.Broadcast();
    }
}
 
void FGameplayTagStackContainer::RemoveStackCount(const FGameplayTag& Tag, int32 Count)
{
    if (Count > 0)
    {
        for (auto It = Stacks.CreateIterator(); It; ++It)
        {
            FGameplayTagStack& Stack = *It;
            if (Stack.Tag == Tag)
            {
                if (Stack.Count <= Count)
                {
                    It.RemoveCurrent();
                    TagCountMap.Remove(Tag);
                    MarkArrayDirty();
                }
                else
                {
                    const int32 NewCount = Stack.Count - Count;
                    Stack.Count = NewCount;
                    TagCountMap[Tag] = NewCount;
                    MarkItemDirty(Stack);
                }
                OnStackChanged.Broadcast();
                return;
            }
        }
    }
}
 
void FGameplayTagStackContainer::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
    for (const int32 Index : RemovedIndices)
    {
        const FGameplayTag Tag = Stacks[Index].Tag;
        TagCountMap.Remove(Tag);
    }
    if (RemovedIndices.Num() > 0)
        OnStackChanged.Broadcast();
}
 
void FGameplayTagStackContainer::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
    for (const int32 Index : AddedIndices)
    {
        const FGameplayTagStack& Stack = Stacks[Index];
        TagCountMap.Add(Stack.Tag, Stack.Count);
    }
    if (AddedIndices.Num() > 0)
        OnStackChanged.Broadcast();
}
 
void FGameplayTagStackContainer::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
    for (const int32 Index : ChangedIndices)
    {
        const FGameplayTagStack& Stack = Stacks[Index];
        TagCountMap[Stack.Tag] = Stack.Count;
    }
    if (ChangedIndices.Num() > 0)
        OnStackChanged.Broadcast();
}

void FGameplayTagStackContainer::AppendTag(FGameplayTag Tag)
{
    AddStackCount(Tag, 1);
}

void FGameplayTagStackContainer::AppendTags(FGameplayTagContainer& Tags)
{
    for (FGameplayTag Tag : Tags)
        AddStackCount(Tag, 1);
}

void FGameplayTagStackContainer::RemoveTag(FGameplayTag Tag)
{
    RemoveStackCount(Tag, 1);
}

void FGameplayTagStackContainer::RemoveTags(FGameplayTagContainer& Tags)
{
    for (const FGameplayTag& Tag : Tags)
        RemoveStackCount(Tag, 1);
}

bool FGameplayTagStackContainer::HasAny(FGameplayTagContainer Tags)
{
    for (FGameplayTag Tag : Tags)
        if (TagCountMap.Contains(Tag) && TagCountMap[Tag] > 0)
            return true;

    return false;
}

FGameplayTagContainer FGameplayTagStackContainer::GetContainerWithoutStacks()
{
    FGameplayTagContainer NewContainer;
    for (auto& TagPair : TagCountMap)
    {
        NewContainer.AddTag(TagPair.Key);
    }
    return NewContainer;
}
