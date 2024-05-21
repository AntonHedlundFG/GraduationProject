# Graduation Project, FutureGames - Game Programmer '22

## Participants
- Anton Hedlund - [Portfolio](https://www.antonhedlund.com/) - [Project Report](Docs/Report/Anton/Report.md)
- Erik Lund - [Portfolio](https://lunderik.wixsite.com/portfolio)
- Johan Brandt - [Portfolio](https://www.johanbrandt.com/)- [Project Report](Docs/Report/Johan/Report_Johan.md)
- Alice Kamil - [Portfolio](https://alicekamil.com/) - [Project Report](Docs/Report/Alice/Report_Alice.md)
- Theo Sandén - [Portfolio](https://www.theosanden.com/)

## Project Goals
This project was intended to serve as a graduation project for a 2 year education at FutureGames Stockholm, for a team of 5 programmers. With no artists or designers involved in the project, the team's goal was not to make a fully completed game, but rather a stable prototype with robust systems, ready for designers and artists to add actual content.

The prototype was pitched as a co-op turn-based tactical rogue-lite with online multiplayer, in Unreal Engine 5, using [Hazelight's UnrealEngine-AngelScript](https://angelscript.hazelight.se/) for gameplay programming, taking inspiration from board games such as *Descent* and *Warhammer: Quest*. 

### Major Features
- Online Multiplayer using Epic Online Services (1-4 players)
- Server-authoritative player input
- Turnbased tactical gameplay
- All player actions are undoable
- Customized logging system for debugging and ingame log
- Procedurally generated levels
- GameplayTag-based Attribute system similar to Unreal's GameplayAbilitySystem
- Enemy and hero units functionally identical, AI uses same functions as player input

![Player takes several actions, changes their mind and undoes the entire turn](Gifs/UndoEntireTurn.gif)
