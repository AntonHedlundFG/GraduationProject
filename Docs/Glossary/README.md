# Glossary

## Commands, Items and Abilities

### Unit
Each Unit represents a pawn (board game, not APawn) in the game. It has no innate information about abilities or attributes, only information about which player or AI controls it.
Each unit instead has a UCAttributeComponent which has information about its state, and a UCInventoryComponent which keeps track of items and their abilities.

### Command
The object representing an ACTIVE ability use from a player or an AI, for example: Attacking with a weapon, Moving with boots.
These are created by Items when the GameMode requests it, and ONLY exist on the server. 

### Consequence
The object representing a PASSIVE effect created as a result of a **Command**. 
For example: 
A trinket which makes all your weapon attacks root enemies triggers when you make an attack. 
A trap that explodes when a unit walks onto it.
A "Thorns" item deals damage to the unit that attacks its wielder.
A unit dies.

### Temporary Effects
These should be implemented within the Execute / Undo functions of either a Command or Consequence.

### Item
A DataAsset which contains the information about how an item works. This does NOT manage any state or information about units. It is Unit-agnostic. 
It grants and removes gameplaytags and changes attributes on the Unit which equips/unequips it, and if it has a specific ItemSlot it can be used by the GameMode to create **Commands** (abilities)

### AttributeComponent
Each Unit has its own AttributeComponent, which contains most information about its state; Health, Range, Shield, as well as which GameplayTags are currently active on the Unit.
Only the server can make changes to the AttributeComponent, but it replicates to Clients for updating VisualEffects.

### VisualEffects
Completely Local object which listens to changes in the AttributeComponent of a Unit, and creates local visual effects when relevant.

### TurnTimerManager
A replacement for the TimerManager in World(), which can create similar functionality but in turn time instead of real time.


## GameMode & other base classes

### GameMode
The GameMode handles Item ability usage. It requests Commands from Items and stores them in the CommandList, which can be undone for the remainder of the ongoing turn, after which they are archived in the CommandHistory.

### PlayerController
The PlayerController handles local input for ability use. Once an ability and a target are chosen locally, an RPC is made to the server which forwards the information to the GameMode for handling.

### GameState
The Turn Order of units is stored in the GameState, but managed by the GameMode.
