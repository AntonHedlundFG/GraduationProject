# Graduation Project Report, Anton Hedlund

## Introduction [TODO: Link to everyone's portfolio]
This is a report regarding a ten week long graduation project at FutureGames Stockholm. The project took place from rom April 18th to May 24th, 2024.

The personal goals I set for my project were:

- Further familiarization with Unreal Engine, and specifically with the AngelScript engine version supported by Hazelight, which is being used in a few different studios in Stockholm.
- Gaining experience in a team with several programmers, as it is going to be a very important aspect of working within the industry, and in our previous Game Projects at FutureGames I have only worked in teams of 2-3 programmers.
- Solidifying my skills in networked gameplay, which has been one of the areas of development I've enjoyed the most, as well as the skill I believe has made me stand out the most among potential internship companies.

As I wanted to work in a team, I was in touch with a few of my co-students who were also interested in a team project within Unreal Engine. We ended up with a team of 5 students; Me, Johan Brandt, Erik Lund, Alice Kamil and Theo Sandén.

## Project Startup
### Goals, team and individual
Very early in our team's discussions, we decided on making a tactical game, something akin to board games like Descent, Gloomhaven or Warhammer: Quest. We decided on a few key features that were core to our game and our respective individual goals for the project:

- Networked multiplayer supporting 1-4 players
- Gameplay does not depend on number of players - if only 1 player plays the game, they control 4 units. For 3 players, the host controls unit 4
- A command-pattern approach to game actions, to allow for a player to undo and reconsider any action taken during their turn
- An MVC pattern approach to visual effects, with server-authoritative game state changes but entirely client-owned visual information
- A procedurally generated game grid board and loot system, using a seed for all random elements.
- Using Unreal Engine's GameplayTag system for programming gameplay
- A unit system where hero units and enemies function identically, so that the AI interacts with the game as a player would. This would build the foundation to potentially introduce "mind-control" effects for both AI and player

### Style guide [TODO: Link to naming conventions]
Before work started on any code, we sat down and decided on a few basic principles for how to style our code consistently across the team.

For example, since we were going to use C++ and AngelScript side-by-side, we decided to name any class implemented natively in C++ with a C following the default Unreal naming convention (i.e. ACMyActor or UCMyObject.) For classes implemented in AngelScript, we instead used an S (ASMyActor, USMyObject). 

## Early week-by-week plans
As part of the first few days of the project being spent on planning our work and deciding on what game we would be making, we established this as a week-by-week plan. Note that this was *preliminary*, and not written in stone.

1. Planning
2. First playable
3. Pre-Alpha
4. Alpha
5. Pre-Beta
6. Beta (Feature Complete)
7. Content
8. Release Candidate
9. Polish
10. Presentation & Report writing

## Mentors [TODO: Ask mentors if I can include their names in report, as well as if they want me to link to their websites or mention anything]
As the school provided a budget for one hour of mentoring per student per week, we decided as a group to reach out to two mentors to divide our weekly five hours between.
### Marc Zaku
Marc was our teacher in the Programming Design Patterns course. Since our game was going to be a very logic-heavy game we needed our core game architecture to be solid, and because of this we believed Marc would be able to do an excellent job as a mentor for the team. 
### Ian Bradley
Ian had mentored our class in a much earlier course on C++, and has plenty of experience working within Unreal Engine. We hoped that as a mentor he could help us with general questions about Unreal, as well as general game development and team issues.

# Weekly Summary [TODO: Add links to other team members' reports?]

This is a weekly summary of my work on the project. I will not go into details about the work other team members did, that information can be found in their respective reports.

## Week 1 [TODO: Add pictures of the whiteboard from week 1]
Much of the first week was spent on various planning meetings including the whole team. The output of these meetings was mostly to align the team members' understanding of the project as a whole, but also a couple of images of whiteboard writings outlining our planned work. Most of the information detailed in [Project Startup](#project-startup) was generated during these meetings.

This week we set up the actual development environment, generating the Unreal project, setting up our GitHub repository, and making sure the entire team had downloaded the AngelScript version of Unreal as developed by Hazelight.

My first few commits included adding a subsystem for hosting and joining sessions through Epic Online Services, which I had made earlier for another project. I also set up the necessary settings to make this work, setting up our Epic Developer Portal and adding the relevant codes to Unreal.

I started working on an initial implementation of the command pattern that was to be used for our gameplay, with a UObject named UCommand for the active behaviour generated by players and AI, and UConsequences which basically function in the same way but which are stored within a UCommand instead. This would make it easy to make sure that when a player undoes their last action, all resulting consequences are undone in exactly the inverse order.

This is a cleaned up version of the initial command class. The BlueprintImplentableEvents named with Receive-prefixes are required to enable AngelScript implementations of the virtual functions, as AngelScript cannot call the native Super:: implementation of an overwritten function.

``` c++
UCLASS(Abstract)
class UCCommand : public UObject
{
public:
	virtual void ExecuteCommand(AController* inController) { ReceiveExecuteCommand();}
	virtual void UndoCommand() { ReceiveUndoCommand(); }
	virtual FString ToString() { return ReceiveToString(); }

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveExecuteCommand();
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveUndoCommand();
	UFUNCTION(BlueprintImplementableEvent)
	FString ReceiveToString();
};
```

### Miscellaneous
- LevelURL asset which keeps a map of URL's to all relevant game levels (Main Menu, Lobby, transition level, Game level), and appends requested settings as optional parameters to the URL.
- GameMode implementation of basic command pattern flow, with client-side PlayerControllers making Server RPC's to activate abilities on the server.
- Default player Pawn, containing only a camera, with movement through WASD keys, edge-of-screen scrolling and by holding down the mouse scroll wheel.
- Basic UI for using abilities, undoing abilities and ending the player's turn, as well as hotkeys. For targeted abilities, I implemented a simple linetrace function for choosing a tile to target.
- Basic movement ability and basic attack ability

## Week 2
Development speed slowed down drastically this week as the team started weaving together many unconnected systems. 

I implemented a basic health system for units, and the first UConsequence implementation, which was UDeathConsequence. It would be created when a unit reached 0 health, and removed the unit from the game board as well as the turn order list. When a UCommand which triggered the UDeathConsequence was undone, the consequence was undone as well, returning the unit to the board where it was, restoring the health it lost, as well as inserting it into the turn order list to its previous position.

This week we also moved away from the basic command pattern flow and implemented a more generic one using UActions. The actions have the same functionality as UCommands and UConsequences, but can also have a relationship with an ActionComponent attached to each Unit. The new UActions also use Unreal Engine's GameplayTags.

We implemented an item system, where each item would contain an ability which can be used to create a series of UActions, as well as an InventoryComponent attached to each Unit, where items could be equipped and removed.

Finally, I implemented a Timer subsystem for our game. Since our game does not play in real time, Unreal Engine's regular real time-based FTimerManager cannot be used for gameplay. Instead my TurnTimer implementation can be used to set a timer to call a delegate function after a specific amount of game turns.

As with all functionality in our game, if an action setting this timer is undone, it must also undo the setting of the timer. The result of a timer finishing can never be undone, as it is not caused directly by a player action.

```c++
UFUNCTION(BlueprintCallable, Category = "Turn Timer")
	FTurnTimerHandle SetTimer(
        const int InNumberOfTurns, 
		ACUnit* InAffectedUnit,
		const bool InbLoop, 
		const EProgressTurnMethod InProgressTurnMethod, 
		FOnTurnTimerExecute InDelegate);

        //Example use which will run SomeFunction at the start of SomeUnit's second next turn. If the third parameter was true, SomeFunction would repeatedly be called every other turn.
        FTurnTimerHandle Handle = 
        SetTimer(2, SomeUnit, false, EProgressTurnMethod::PTM_StartOfTurn, &UThisClass::SomeFunction)
```

## Week 3 [TODO: Add the sequence diagram from https://github.com/AntonHedlundFG/GraduationProject/blob/2e2ace216abbdb67012a704ce95e1fccaf7cdae9/Docs/SequenceDiagrams/ActionSequenceDiagram.md?plain=1]

Almost the entire week was spent on recreating old functionality in the new system using UActions. It was a somewhat frustrating week with a feeling of "one step forward, two steps back", but by the end of it almost everything was back in place, and much improved.

I spent some time this week researching how to make Unreal Engine's UObjects replicate properly, as they do not do so by default, as well as how replicated TArrays of pointers to replicated UObjects work, and how often they trigger their respective OnRepNotify functions.

I was surprised to see that when creating a new replicated UObject and adding it to a replicated TArray in the same frame, the Array's OnRepNotify function is still called twice; first to notify the client of the array's increased size, then to actually validate the newly added pointer.

Finally, implementing the new UAction system was very difficult, and I had a hard time communicating the workflow to the rest of the team. Our mentor Marc suggested I write a sequence diagram using mermaid to describe the information flow. I had never made one before, but this is my implementation:

[INSERT DIAGRAM HERE]

## Week 4



## Week 5

## Week 6

## Week 7

## Week 8

## Week 9

## Week 10


# [TODO: Decide on end-of-project outline. Outcome & Learnings?]