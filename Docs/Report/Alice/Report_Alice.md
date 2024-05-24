# Graduation Project Report - Alice Kamil



## Keywords

**Engine:** Unreal Engine 5.3, C++, AngelScript.

**Tools:** Jira, Doxygen.

**Duration:** 10 weeks.

**Team size:** 5 programmers.

# Project Info

A networked multiplayer, turn-based tactical rouge-lite game using Unreal Engine 5. This project also leverages [Hazelight's modified engine with Angelscript](https://angelscript.hazelight.se/) for scripting capabilities. The online sessions are managed through [Epic Online Services](https://dev.epicgames.com/en-US/services) for Unreal. 

# Work Process

I worked in a team of 5 and we all dedicated 40 hours per week on this project, utilising agile project management practices, including daily standups, weekly sprints and continuous bug reporting. Our workflow was enhanced by weekly meetings with two mentors- Marc Zaku and Ian Bradley- who provided invaluable playtesting feedback and guidance for us. The overall process felt very organised, and my teammates were all devoted and committed to this project. 

**We established our weekly progress goals early on and felt confident in fulfilling them:**

**Week 1:** Planning and project setup

**Week 2:** First playable

**Week 3:** Pre-Alpha

**Week 4:** Alpha

**Week 5:** Pre-Beta

**Week 6:** Beta (feature-complete)

**Week 7:** Content development

**Week 8:** Release Candidate

**Week 9:** Polish

**Week 10:** Wrap-up and presentation

This only served as a outline of how we wanted to progress week by week. Wether we kept this timeframe or not can be found under [time estimation](#time-estimate).

# Goals

Creating a complete, launchable game was always a secondary objective for me in this project. My primary goal was to grow as a gameplay programmer and to learn more advanced practices for building a robust gameplay framework.

### **In my project plan, I outlined some key challenges that I aimed to overcome:**

**1. Building a user-friendly framework for designers:**

 * Although this project didn't involve any designers, I aimed to practice developing a system that would make it easy for them to create and tweak abilities without requiring any additional coding.
 I believe this is essential for a good gameplay programmer and something I strive to master, as the rewarding aspect for me in this role is to enable others to work more efficiently and creatively- without being bogged down by technical challenges.


**2. Creating an efficient execution flow for actions:**

* Making sequential actions that can be undone and trigger additional actions as reactions would require a robust foundation. Managing the lifetime of these actions in terms of memory and garbage collection also presented a big challenge, but first and foremost- a good learning opportunity.  

**3. Balancing abilities:**

* This was quite a secondary goal, as it is an area game designers spend years iterating on. More importantly, my aim was to make the iteration and adjustments of the actions straightforward. Success in this endeavor meant minimizing the need for code changes, enabling designers to fine-tune and balance abilities independently. The fewer issues that required code modifications, the more successful I considered this goal to be.

## Internal Goals

### **In addition to these challenges, I also set some personal goals  for myself:**

 
**1. Requiring more responsibility during a school project:**
* I often felt like the least experienced team member and tended to shy away from taking up space. This time, I aimed to step up and develop a bigger feature in the game. 

**2. Daring to ask questions and be transparent with my teammates:**
* I've struggled with low self-esteem and feared that others might think I'm incompetent. By daring to expose this and ask questions, I wanted to get more comfortable and confident in a collaborative environment.

**3. Getting familiar with AngelScript**.


# Contributions

I had tried to gain knowledge about the [Gameplay Ability System](https://dev.epicgames.com/documentation/en-us/unreal-engine/gameplay-ability-system-for-unreal-engine?application_version=5.3) in Unreal and was particularly interested in [Tom Looman's](https://github.com/tomlooman/ActionRoguelike) approach, which offers a more light-weight version of GAS. I strongly advocated integrating concepts from these systems in our project, as they suited the game we wanted to create and served as excellent learning resources.

By leveraging his methods, I think I reached my goal in familiarizing myself to more advanced Unreal Engine concepts- such as asynchronous data asset loading, the efficient use of Gameplaytags, and working with soft references.

### **Here are the contributions I made, in collaboration with my teammates:**


#### - Framework for [actions]( #actions). 

#### - Custom native [Gameplaytags](#custom-gameplaytags).

#### - [Attribute System](#attribute-management).

#### - [Itemdata](#items-using-primarydataassets) using primary data assets.

#### - Async loading of [ability UI icons](#soft-references-and-async-loading).


#### - Custom [Gameplaytag stack container](#--custom-gameplaytag-count-container-to-be-able-to-stack-and-replicate-the-tags) with Fast TArray Replication. //CHECK LINK

 
#### - [GameplayFunctionLibrary](#--a-gameplay-function-library-for-applying-damage) for damage application. //CHECK LINK

#### - Weighted [itemrolling Subsystem](#--an-itemrolling-subsystem) using data tables. //CHECK LINK

#### - Custom [Asset Manager](#--our-own-asset-manager). //CHECK LINK

#### - [Achievement System](#--async-loaded-achievement-system--not-in-use) with async asset loading. //CHECK LINK

#### - [Charms](#--charm-items-with-passive-actions-and-stacking-possibilites-using-slots) and passive item-actions. //CHECK LINK

#### - Event-based [health and effect UI](#actorwidget). //CHECK LINK



## Actions

At the heart of the gameplay is the action system, allowing players to execute a wide range of [actions](https://github.com/AntonHedlundFG/GraduationProject/blob/main/TacticalRogueLite/Source/TacticalRogueLite/Private/Actions/CAction.cpp)- from basic movement to more complex, interconnected abilities- all of which can be undone and reconsidered within the same turn. Each action is encapsulated, making it easy to start, stop and revert them in a command-pattern approach.

_"Start Action"-function in the base class:_
```c++
void UCAction::StartAction(AActor* Instigator)
{
    RepData.bIsRunning = true;
	RepData.Instigator = Instigator;

    //Applying all editor-defined attribute changes on the unit upon action activation.
    for (FAttributeModification& Mod : ModifiersAppliedToOwner)
	{
		int ActualDelta = GetOwningComponent()->ApplyAttributeChange(Mod, 0);
		ModifiersActualDeltas.Add(ActualDelta);
	}
	ReceiveStartAction(Instigator);
}
```

_Example of "Start Action" in inherited class:_
```c++
void UCAttackAction::StartAction(AActor* Instigator)
{
	Super::StartAction(Instigator);
	
    //Retrieve the units current attackdamage...
	FAttribute Attribute;
	GetOwningComponent()->GetAttribute(FGameplayTag::RequestGameplayTag("Attribute.AttackDamage"), Attribute);
    //...Apply that damage to the target...
	ReturnModifications = UCGameplayFunctionLibrary::ApplyDamage(Instigator, Target, Attribute.BaseValue, ActionTags);

    //...And apply all the additional effects this action will generate to that specific target.
    for (FAttributeModification Mod : ReturnModifications.Modifications)
	{
		Target->GetActionComp()->ApplyAttributeChange(Mod, 0);
    }
} 
```

Leveraging [Unreal's Gameplaytags](https://dev.epicgames.com/documentation/en-us/unreal-engine/gameplay-framework-in-unreal-engine?application_version=5.3), all actions are populated with "blocking tags" and "action tags", which are set in the editor. As the names might suggest, these serve as conditionals for the actions to be allowed to execute.

Rather than dealing with numerous bools, we pass the actions "action tags"-which serves to give context- to the units "Action Component". This approach not only provides contextual clarity but also simplifies management compared to casting actors with varied boolean variables.

![TagsOnActions](https://github.com/AntonHedlundFG/GraduationProject/blob/main/Images/Tags.png)

```c++
bool UCAction::CanStart(AActor* Instigator)
{
	if (IsRunning())
	{
		return false;
	}
	
	UCActionComponent* Comp = GetOwningComponent();
	
	if (Comp->ActiveGameplayTags.HasAny(ActionBlockingTags) && ReceiveCanStart(Instigator))
	{
		return false;
	}
	return true;
}
```

## Action Component

The [Action Component](https://github.com/AntonHedlundFG/GraduationProject/blob/main/TacticalRogueLite/Source/TacticalRogueLite/Private/Actions/CActionComponent.cpp) serves as the core for adding, removing and registering the actions and abilities. As mentioned, the component serves as a "state machine", housing current actions along with the [associated tags](https://github.com/AntonHedlundFG/GraduationProject/blob/f1dbc4f6ae4332b529bb7737d888b9c52f88ca47/TacticalRogueLite/Source/TacticalRogueLite/Public/Actions/CActionComponent.h#L55).

The actions also has an "Activation Tag" which is not currently in use, but is meant as a "nickname" to it's action, enabling it to be started without any hard object reference to the object.

```c++
    //Eg. "Action_Rooting".
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Action")
	FGameplayTag ActivationTag;
```  
```c++
bool UCActionComponent::StopActionByName(AActor* Instigator, FGameplayTag ActionName)
{
	for (UCAction* Action : Actions)
	{
		if (Action->GetActivationTag() == ActionName)
		{
			Action->StopAction(Instigator);
			return true;
		}
	}

	return false;
}
```

## Attribute Management

In addition to managing actions, the Action Component also oversees the unit's [attributes](https://github.com/AntonHedlundFG/GraduationProject/blob/main/TacticalRogueLite/Source/TacticalRogueLite/Public/Attributes/Utilities/CAttribute.h), which are stored in an [AttributeSet](https://github.com/AntonHedlundFG/GraduationProject/blob/main/TacticalRogueLite/Source/TacticalRogueLite/Private/Attributes/CAttributeSet.cpp). Each attribute- such as health, armor or attackdamage- is also associated with a distinct gameplaytag, again, to not have to rely on any casts or hard object references.

```c++
bool UCAttributeSet::GetAttribute(FGameplayTag InTag, FAttribute& OutAttribute)
{

	FName PropertyName;
	if(GetAttributeName(InTag, PropertyName))
	{
		FStructProperty* Prop = CastField<FStructProperty>(GetClass()->FindPropertyByName(PropertyName));
		if (Prop)
		{
			FAttribute* FoundAttribute = Prop->ContainerPtrToValuePtr<FAttribute>(this);
			OutAttribute = *FoundAttribute;
			return true;
		}
	}

	//Failed to find attribute based on gameplaytag.
	return false;
}
```



## Tag-driven Broadcasting

One of the key advantages of the framework in my opinion is the ability to dynamically impact gameplay in response to attribute [modifications](https://github.com/AntonHedlundFG/GraduationProject/blob/84ca0354d9e61e27906808ebe84be539e1dcd8b0/TacticalRogueLite/Source/TacticalRogueLite/Public/Attributes/Utilities/CAttribute.h#L109). By [broadcasting](https://github.com/AntonHedlundFG/GraduationProject/blob/84ca0354d9e61e27906808ebe84be539e1dcd8b0/TacticalRogueLite/Source/TacticalRogueLite/Private/Actions/CActionComponent.cpp#L140) attribute changes using tags, we can trigger corresponding actions or events in a flexible way.

_Adding an attribute listener:_
```c++
void UCActionComponent::AddAttributeChangedListener(FGameplayTag AttributeTag, const FAttributeChangedSignature& Event,
                                                    bool bCallNow)
{
	//Create and add new pair to listen for.
	new(AttributeChangeTriggers) TPair<FGameplayTag, FAttributeChangedSignature>(AttributeTag, Event);

	//Convenient for Blueprint that may get bound late and want to get the 'initial' state to apply.
	if (bCallNow)
	{
		FAttribute Attribute;
		//May still be nullptr initally for clients during init.
		if (AttributeSet)
		{
			AttributeSet->GetAttribute(AttributeTag, Attribute);
		}

		FGameplayTagContainer EmptyContainer;

		Event.Execute(this, this, AttributeTag, Attribute.GetValue(), Attribute.GetValue(), EmptyContainer, EAttributeModifierOperation::Invalid);
	}
}
```
_Applying attribute changes:_
```c++
int UCActionComponent::ApplyAttributeChange(const FAttributeModification& InAttributeMod, int32 Level)
{
	//Applies the attribute, triggers events and allows other attributes to react to the change.
	//eg. +3 Strength might change the HealthMax too.
	if (AttributeSet)
	{
		return AttributeSet->ApplyAttributeChange(InAttributeMod, Level);
	}
	return 0;
}
```

The actual modification, broadcasting and post "reactions" all happen in the [ApplyAttributeChange](
https://github.com/AntonHedlundFG/GraduationProject/blob/f1dbc4f6ae4332b529bb7737d888b9c52f88ca47/TacticalRogueLite/Source/TacticalRogueLite/Private/Attributes/CAttributeSet.cpp#L71) function on the Attributeset.


## Items using PrimaryDataAssets

To address the core needs sourrounding [items](https://github.com/AntonHedlundFG/GraduationProject/blob/main/TacticalRogueLite/Source/TacticalRogueLite/Public/ItemData/CItemData.h) in our game, we required a versatile, yet generic way to define and load them. We utilized [primary data assets](https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/Engine/Engine/UPrimaryDataAsset?application_version=5.4) to build these. These assets are designed to solely contain raw data without any functional logic, serving as descriptors. 

_Example of an item:_

![ItemData](https://github.com/AntonHedlundFG/GraduationProject/blob/main/Images/itemdata.png)



Primary data assets act as glorified FNames, with their IDs categorized and identified through Unreal's [Asset Manager](https://dev.epicgames.com/documentation/en-us/unreal-engine/asset-management-in-unreal-engine?application_version=5.3). This system allows us to asynchronously load parts of the asset on demand, optimizing memory usage. 

#### Asset Bundles

 Asset bundles can be used to categorize the soft references inside the primaryasset to a specific type or use-case, just like I'm doing here with all the itemdata's UI.

```c++
//Access in blueprints via 'get as texture/material' helpers to allow easy soft-loading which can't be done through base class.
	UPROPERTY(EditDefaultsOnly, Category = "UI", meta = (AssetBundles = "UI", AllowedClasses = "MaterialInterface, Texture2D"))
	TSoftObjectPtr<UObject> Icon;
```

#### Soft references and async loading

This was particularly useful when it came to the Ability UI- instead of loading all the related itemdata, we could just async load the things I've had marked up with "UI"!


![AsyncLoad](https://github.com/AntonHedlundFG/GraduationProject/blob/main/Images/asyncload.png)


_Example of another primaryasset usecase_:
```c++
//Utility to find any Data class through its OwnedTags without having to load every asset(using Asset Registry).
	UFUNCTION(BlueprintCallable, Category = "AssetRegistry")
	static FPrimaryAssetId GetItemDataByTag(FGameplayTagContainer InTags);
```



### Other features w/ a link to some of the related code: 

#### - An [Itemrolling SubSystem](https://github.com/AntonHedlundFG/GraduationProject/blob/main/TacticalRogueLite/Source/TacticalRogueLite/Private/GamePlay/Subsystems/CItemRollingSubSystem.cpp).

#### - A [Loottable](https://github.com/AntonHedlundFG/GraduationProject/blob/b0d7b88135665052e5e8686f65d6815edb66258a/TacticalRogueLite/Source/TacticalRogueLite/Public/ItemData/UtilityQuery/CTypes.h#L19) and gaussian randomness(which was left out).

#### - Charm items with [passive actions](https://github.com/AntonHedlundFG/GraduationProject/blob/b0d7b88135665052e5e8686f65d6815edb66258a/TacticalRogueLite/Source/TacticalRogueLite/Private/ItemData/CItemData.cpp#L32) and [stacking](https://github.com/AntonHedlundFG/GraduationProject/blob/b0d7b88135665052e5e8686f65d6815edb66258a/TacticalRogueLite/Source/TacticalRogueLite/Private/Items/CInventoryComponent.cpp#L75) possibilites using [slots](https://github.com/AntonHedlundFG/GraduationProject/blob/b0d7b88135665052e5e8686f65d6815edb66258a/TacticalRogueLite/Source/TacticalRogueLite/Public/Items/CInventoryComponent.h#L18-L20).

#### - Custom [Gameplaytag count container](https://github.com/AntonHedlundFG/GraduationProject/blob/b0d7b88135665052e5e8686f65d6815edb66258a/TacticalRogueLite/Source/TacticalRogueLite/Private/Attributes/Utilities/CAttributeUtilities.cpp) to be able to stack and replicate the tags. 

#### - Our own [Asset Manager](https://github.com/AntonHedlundFG/GraduationProject/blob/b0d7b88135665052e5e8686f65d6815edb66258a/TacticalRogueLite/Source/TacticalRogueLite/Private/Assets/AssetManager/CAssetManager.cpp).

#### - A [Gameplay Function Library](https://github.com/AntonHedlundFG/GraduationProject/blob/b0d7b88135665052e5e8686f65d6815edb66258a/TacticalRogueLite/Source/TacticalRogueLite/Private/Attributes/CGameplayFunctionLibrary.cpp) for applying damage.

#### Custom [Gameplaytags](https://github.com/AntonHedlundFG/GraduationProject/blob/main/TacticalRogueLite/Source/TacticalRogueLite/Private/GamePlayTags/SharedGamePlayTags.cpp).

#### - Base [ActorWidget](https://github.com/AntonHedlundFG/GraduationProject/blob/main/TacticalRogueLite/Source/TacticalRogueLite/Private/Widgets/CActorWidget.cpp).

#### - Async loaded [Achievement System](https://github.com/AntonHedlundFG/GraduationProject/blob/b0d7b88135665052e5e8686f65d6815edb66258a/TacticalRogueLite/Source/TacticalRogueLite/Private/Achievements/CAchievementSubsystem.cpp)- not in use.

#### - Eventdriven health UI.



# Challenges


The biggest challenge of them all was trying to understand this huge framework and decide what I should leverage and what to leave out. GAS is a complicated beast to say the least, with layers upon layers of abstraction. In my opinion, this is what made the challenge _fun_, but I should've kept in mind that I shouldv'e  I had other programmers that was dependant on the systems I developed, and maybe should've iterated faster by introducing smaller part of the framework than all at once. 

 The challenge around the framework also included remaking it from something that was aimed for real time to our turn-based, "revertable" gameplay. I had another programmer do the execution order and lifetime handling of the actions, but the action system works more or less the same as in a realtime game- with the exception that every action can be reverted of course.


  The second biggest challenge was to sell this to my teammates. I don't know how the project would end up if we didn't go with this system, maybe we’d progress faster, since it took a lot of time to understand, but I'm proud of myself for pushing what I want, and taking ownership of key features, which alligned with my [internal goals](#internal-goals). As mentioned there, the most important thing for me in this final project was to gain knowledge and learn more advanced concepts, which I definitely think I did. 



## Time estimate

As mentioned.... bla bla

# Skills and Knowledge

**-Gained understanding of the Gameplay Ability System.**

**-Learnt more about Unreals property handling.**

**-Leveraged Primary data assets and the Asset manager.**

**-Learnt how to build eventdriven gameplay with GameplayTags.**

**-Became more aware of different profiling tools in Unreal.**

**-Learnt about Fast serializer array's.**

**-Introduced myself to replication.**
