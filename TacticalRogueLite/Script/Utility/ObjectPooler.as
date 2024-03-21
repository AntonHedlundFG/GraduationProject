
class USObjectPool : UObject
{
    UPROPERTY(VisibleAnywhere)
    TSubclassOf<AActor> ObjectClass;

    UPROPERTY(VisibleAnywhere)
    TArray<AActor> ObjectPool;

    UFUNCTION()
    void Initialize(TSubclassOf<AActor> InObjectClass, int64 StartSize = 0)
    {
        if(InObjectClass == nullptr)
        {
            Print("ObjectClass is null, please set ObjectClass before initializing the pool");
            return;
        }
        ObjectClass = InObjectClass;

        for (int32 i = 0; i < StartSize; i++)
        {
            ObjectPool.Add(CreatePoolableActor());
        }
    }

    UFUNCTION()
    AActor Dequeue(bool bIsActive = true)
    {
        AActor obj;
        if(ObjectPool.Num() == 0)
        {
            obj = CreatePoolableActor();
        }
        else
        {
            obj = ObjectPool[0];
            ObjectPool.RemoveAt(0);
        }
        ASPoolableActor poolableActor = Cast<ASPoolableActor>(obj);
        if(poolableActor != nullptr)
        {
            poolableActor.OnDequeue();
        }
        obj.SetActorHiddenInGame(bIsActive);
        obj.SetActorTickEnabled(bIsActive);
        return obj;
    }
    
    UFUNCTION()
    void Enqueue(AActor Actor,bool bIsActive = false)
    {
        if(!Actor.Class.IsChildOf(ObjectClass))
        {
            Print(f"Trying to Enqueue {Actor.Class} which is not type of {ObjectClass}.");
            return;
        }
        if(ObjectPool.Contains(Actor))
        {
            Print(f"{Actor.Name} Already Enqueued.");
            return;
        }
        ASPoolableActor poolableActor = Cast<ASPoolableActor>(Actor);
        if(poolableActor != nullptr)
        {
            poolableActor.OnEnQueue();
        }
        Actor.SetActorHiddenInGame(bIsActive);
        Actor.SetActorTickEnabled(bIsActive);
        ObjectPool.Add(Actor);
    }

    UFUNCTION()
    void ClearPool()
    {
        for (AActor Object : ObjectPool)
        {
            if(IsValid(Object))
            {
                Object.DestroyActor();
            }
        }
        ObjectPool.Empty();
    }

    UFUNCTION()
    AActor GetAt(bool bIsActive, FVector Location, FRotator Rotator)
    {
        AActor Actor = Dequeue(bIsActive);
        Actor.SetActorLocation(Location);
        Actor.SetActorRotation(Rotator);
        return Actor;
    }
    UFUNCTION()
    private AActor CreatePoolableActor(bool bIsActive = false)
    {
        AActor Actor = Cast<AActor>(SpawnActor(ObjectClass, FVector::ZeroVector, FRotator::ZeroRotator));
        Actor.SetActorHiddenInGame(bIsActive);
        Actor.SetActorTickEnabled(bIsActive);
        return Actor;
    }

}

event void FOnPoolEnterExit();
class ASPoolableActor : AActor
{
    USObjectPool pool;

    FOnPoolEnterExit onEnQueue_evnt;
    FOnPoolEnterExit onDequeue_evnt;

    void SubscribePool(USObjectPool pool)
    {
        this.pool = pool;
    }
    void OnEnQueue()
    {
        onEnQueue_evnt.Broadcast();
    }
    void OnDequeue()
    {
        onDequeue_evnt.Broadcast();
    }
}

class UObjectPoolSubsystem : UWorldSubsystem
{
    UPROPERTY(VisibleAnywhere)
    private TMap<TSubclassOf<AActor>, USObjectPool> ObjectPools;

    UFUNCTION()
    USObjectPool CreatePool(TSubclassOf<AActor> InObjectClass, int64 initialSize)
    {
        if(ObjectPools.Contains(InObjectClass))
        {
            Print(f"Object Pool Already Exists for object of type{InObjectClass}.");
            return ObjectPools[InObjectClass];
        }
        USObjectPool objectPool = Cast<USObjectPool>(NewObject(this,USObjectPool::StaticClass()));
        objectPool.Initialize(InObjectClass,initialSize);
        return objectPool;
    }
    void ClearPool(TSubclassOf<AActor> InObjectClass)
    {
        if(ObjectPools.Contains(InObjectClass))
        {
            ObjectPools[InObjectClass].ClearPool();
        }
    }
    void DeletePool(TSubclassOf<AActor> InObjectClass)
    {
        if(ObjectPools.Contains(InObjectClass))
        {
            USObjectPool pool = ObjectPools[InObjectClass];
            pool.ClearPool();
            ObjectPools.Remove(InObjectClass);
        }
    }
    UFUNCTION()
    USObjectPool GetPool(TSubclassOf<AActor> InObjectClass)
    {
        if(ObjectPools.Contains(InObjectClass))
        {
            return ObjectPools[InObjectClass];
        }
        return CreatePool(InObjectClass,0);
    }

    AActor DequeueActor(TSubclassOf<AActor> InObjectClass,bool bIsActive, FVector Location = FVector::ZeroVector, FRotator Rotation = FRotator::ZeroRotator)
    {
        USObjectPool pool = GetPool(InObjectClass);
        return pool.GetAt(bIsActive,Location,Rotation);
    }

    void EnqueueActor(TSubclassOf<AActor> InObjectClass, AActor Actor, bool bIsActive = false)
    {
        if(!Actor.Class.IsChildOf(InObjectClass))
        {
            Print(f"{Actor.Name} is not a subclass of {InObjectClass}.");
            return;
        }
        ObjectPools[InObjectClass].Enqueue(Actor,bIsActive);
    }
}