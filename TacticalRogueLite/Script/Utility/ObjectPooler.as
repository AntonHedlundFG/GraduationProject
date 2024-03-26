
class USObjectPool : UObject
{
    UPROPERTY(VisibleAnywhere)
    TSubclassOf<AActor> ObjectClass;

    UPROPERTY(VisibleAnywhere)
    TArray<AActor> ObjectPool;

    //Initializes the pool with an amount of objects, Calling dequeue will create objects aswell.
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
    //Retrieves an object from the pool, if none are avaliabe it creates a new one.
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
        obj.SetActorHiddenInGame(!bIsActive);
        obj.SetActorTickEnabled(bIsActive);
        return obj;
    }
    //Enqueues and object to the pool
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
        Actor.SetActorHiddenInGame(!bIsActive);
        Actor.SetActorTickEnabled(bIsActive);
        ObjectPool.Add(Actor);
    }
    //Deletes all objects that are in the pool
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
    //Returns an object and sets it position and rotation
    UFUNCTION()
    AActor DequeueAt(bool bIsActive, FVector Location, FRotator Rotator)
    {
        AActor Actor = Dequeue(bIsActive);
        Actor.SetActorLocation(Location);
        Actor.SetActorRotation(Rotator);
        return Actor;
    }
    //Creates a new pool object
    UFUNCTION()
    private AActor CreatePoolableActor(bool bIsActive = false)
    {
        AActor Actor = Cast<AActor>(SpawnActor(ObjectClass, FVector::ZeroVector, FRotator::ZeroRotator));
        ASPoolableActor poolableActor = Cast<ASPoolableActor>(Actor);
        if(poolableActor != nullptr)
        {
            poolableActor.SubscribePool(this);
        }
        Actor.SetActorHiddenInGame(bIsActive);
        Actor.SetActorTickEnabled(bIsActive);
        return Actor;
    }

}

event void FOnPoolEnterExit();
//A class that has functions that are being called when the object moves in and out of a pool
class ASPoolableActor : AActor
{
    USObjectPool pool;

    FOnPoolEnterExit onEnQueue_evnt;
    FOnPoolEnterExit onDequeue_evnt;

    //This is called when the pool object is created
    void SubscribePool(USObjectPool inPool)
    {
        pool = inPool;
    }
    //This is called when the object is put back into the pool
    void OnEnQueue()
    {
        onEnQueue_evnt.Broadcast();
    }
    //This is called when the object is removed from the pool
    void OnDequeue()
    {
        onDequeue_evnt.Broadcast();
    }
    //Call this to enqueue object into the pool without a reference to the pool
    UFUNCTION(BlueprintCallable)
    void EnqueueSelf()
    {
        Print("" + this.Class);
        pool.Enqueue(this,false);
    }
}

//In order to get access this call UObjectPoolSubsystem::Get();
class UObjectPoolSubsystem : UWorldSubsystem
{
    UPROPERTY(VisibleAnywhere)
    private TMap<TSubclassOf<AActor>, USObjectPool> ObjectPools;
    //Creates a pool and initializes it with a number of objects
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
    //Clear the pool from all objects, these objects are deleted
    void ClearPool(TSubclassOf<AActor> InObjectClass)
    {
        if(ObjectPools.Contains(InObjectClass))
        {   
            ObjectPools[InObjectClass].ClearPool();
        }
    }
    //Deletes the pool entierly as well as all objects within
    void DeletePool(TSubclassOf<AActor> InObjectClass)
    {
        if(ObjectPools.Contains(InObjectClass))
        {
            USObjectPool pool = ObjectPools[InObjectClass];
            pool.ClearPool();
            ObjectPools.Remove(InObjectClass);
        }
    }
    //Gets the pool of passed object, will create a pool if none exists
    UFUNCTION()
    USObjectPool GetPool(TSubclassOf<AActor> InObjectClass)
    {
        if(ObjectPools.Contains(InObjectClass))
        {
            return ObjectPools[InObjectClass];
        }
        return CreatePool(InObjectClass,0);
    }
    //Retrives an object from a pool, if no pool exists, it creates a pool for that object, Use GetPool and call dequeue from it instead if you're doing multiple calls.
    AActor DequeueActor(TSubclassOf<AActor> InObjectClass,bool bIsActive, FVector Location = FVector::ZeroVector, FRotator Rotation = FRotator::ZeroRotator)
    {
        USObjectPool pool = GetPool(InObjectClass);
        return pool.DequeueAt(bIsActive,Location,Rotation);
    }
    //Enqueuea an object to a pool. Use GetPool and call enqueue from it instead if you're doing multiple calls.
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