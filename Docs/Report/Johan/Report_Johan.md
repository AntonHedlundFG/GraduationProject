# Graduation Project Report, Johan Brandt

## Introduction
This is the report for my graduation project at Futuregames in Stockholm, undertaken from April 18 to May 24. This graduation project provided an opportunity to explore and practice with the capabilities of Unreal Engine while integrating it with Hazelight's AngelScript. 
Through this report, I will detail the process, challenges, and learning outcomes of my graduation project.

The project was made as a team effort with five programming students; [Me](https://www.johanbrandt.com/), [Anton Hedlund](https://www.antonhedlund.com/), [Erik Lund](https://lunderik.wixsite.com/portfolio), , [Alice Kamil](https://alicekamil.com/), [Theo Sandén](https://www.theosanden.com/).

### Project Goals
My primary goals for the project were as follows:

- **Enhance proficieny with Unreal Engine:** To practice and advance my familiarity with Unreal Engine.
- **C++ and Scripting Integration:** To practice and advance my skills in C++ along with a scripting language, Angelscript in this case. 
- **Team Collaboration:** To gain experience working with a larger programmer team of five memeber, compared to my previous experience with a maximum of three. 
- **Networking in Games:** To develop more hands-on experience working with  networking in games. 
- **AI System Development:** To build and develop a robust base for the game's AI that would support all the possible behaviors needed for the game, both now and in future iterations of the project.

### Project scope and Significance
For this project, I have worked in a team of five programmers to develop a turn-based tectical rogue-lite game with optional cooperative multiplayer features over a network session. Online session management was handles using Epic Online Services for Unreal Engine. We utilized a modified version of Unreal Engine 5.3.0, which included support for Hazelights' AngelScript, to build the game.

### Individual and Team Goals
Our goal was to create a tactical, almost board game-like experience, influenced by games like Descent, Gloomhaven and Warhammer: Quest. We also aimed to incorporate elements from procedurally generated roguelite games such as The Binding of Isaac, FTL: Faster Than Light, and Hades.

The key pillars for the game that we agreed upon were:
- Procedural generation for loot and game grid. 
- A seeded approach to the randomness in the game.
- Command-pattern for game actions, with the ability to redo all actions made by the players.
- Units that function identically for both the player and for the AI. The AI should interact with the game in the exact same way that the player and have access to all the same information as the player.
- Units have no inherent abilities and stats;  all abilities and stats were to come from the items.
- Networked multiplayer support for up to four players.
- Server authorative control with MVC pattern for visuals where the clients handle their own visual information.
- Gameplay that is agnostic to the player count; fewer players would simply mean that each player controls more units.

### Early design
The game was designed to operate on a square grid where players take turns with AI to control character units. Players shared control over four characters, with each player able to manage multiple characters if there were fewer than four players. All units' stats and abilities were to be granted by items, leaving no innate abilities for the units except for the ability to use items. Items could be slot-specific, granting abilities, or slot-less, providing passive effects. For example, boots might grant a movement ability, a weapon might grant an attack ability, and armor could increase health value. Slot-less items could instead grant passive increases to, for example movement range or attack.

Enemy units were designed to be identical to player-controlled units, differing only in the controlling entity, which simplified integration and interaction within the game environment. It would also give a sense of fairness to the game and inherently show the player how the AI controlled units would be able to act.

Given the absence of dedicated artists, we adopted a minimalistic art approach using basic geometric shapes and billboards for the game actors. Additionally, we tried to emphasize the use of scalable and extendable system architecture. The plan was to use this approach to content generation as much as possible, given the lack of dedicated designers. The goal was to create a lot of content using systems instead of creating it by hand.

### Anticipated Challenges
One of the largest challenges I anticipated was to come from working in a team of five programmers without the support of dedicated artists or designers. Previously, I had worked with a maximum of three other programmers, and even in that smaller group, task allocation and collaboration often presented difficulties. Organizing our team to avoid these issues as much as possible would be crucial. 
Additionally, the absence of designers meant that we needed to architect our systems to be scalable and easily extendable. The systems had to be robust from the start, allowing for efficient content addition throughout the project and potential expansion later on. This required careful planning and systematic content generation, which would be a key focus as the project unfolded.

### [Style guide](https://github.com/AntonHedlundFG/GraduationProject/blob/main/Docs/Naming%20Conventions/README.md)
During the planning phase of the project, we established specific naming conventions. Some of these conventions are standard, while others are unique to our project.
For example, while following the default Unreal naming conventions, we also agreed on additional rules: all native C++ classes would start with a "C" (e.g. CMyActor), and all AngelScript classes would start with an "S" (e.g. SMyActor).

## Work Process

## Goal Achievement

## Time Management

## Skills and Knowledge Gained

## Challenges and Solutions

## Project Analysis

## Conclusion

## References

