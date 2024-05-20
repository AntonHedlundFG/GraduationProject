# Graduation Project Report, Johan Brandt

## Introduction
This is the report for my graduation project at Futuregames in Stockholm, undertaken from April 18 to May 24. This graduation project provided an opportunity to explore and practice with the capabilities of Unreal Engine while integrating it with Hazelight's AngelScript. 
Through this report, I will detail the process, challenges, and learning outcomes of my graduation project.

The project was made as a team effort with five programming students; [Me](https://www.johanbrandt.com/), [Anton Hedlund](https://www.antonhedlund.com/), [Erik Lund](https://lunderik.wixsite.com/portfolio), , [Alice Kamil](https://alicekamil.com/), [Theo Sandén](https://www.theosanden.com/).

### Project scope and Significance
For this project, I have worked in a team of five programmers to develop a turn-based tectical rogue-lite game with optional cooperative multiplayer features over a network session. Online session management was handles using Epic Online Services for Unreal Engine. We utilized a modified version of Unreal Engine 5.3.0, which included support for Hazelights' AngelScript, to build the game.

### Individual Project Goals
My primary goals for the project were as follows:

- **Enhance proficieny with Unreal Engine:** To practice and advance my familiarity with Unreal Engine.
- **C++ and Scripting Integration:** To practice and advance my skills in C++ along with a scripting language, Angelscript in this case. 
- **Team Collaboration:** To gain experience working with a larger programmer team of five memeber, compared to my previous experience with a maximum of three. 
- **Networking in Games:** To develop more hands-on experience working with  networking in games. 
- **AI System Development:** To build and develop a robust base for the game's AI that would support all the possible behaviors needed for the game, both now and in future iterations of the project.

### Team Project Goals
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

### Initial Week-by-week Plan
In the initial planning of the project we laid out a flexible plan with milestone for each week. The plan served as a guide rather than a fixed schedule, allowing us to adapt as needed throughout the project.

1. Planning and project setup
2. First playable
3. Pre-Alpha
4. Alpha
5. Pre-Beta
6. Beta
7. Content development
8. Release Candidate
9. Polishing
10. Wrap-up, presentation

### Project Management and Communication
The team will use Discord for online communication, GitHub for version control and Jira for task management. We will have a stand-up meeting, daily,at 10 AM. We won't have weekly reviews of the project, but we will make sure to have a weekly build from week 2 and onwards. Review meetings for the whole team will be planned and scheduled for when there is a need.

### Mentorship and Guidance
The school provided us with a budget of 1 hour or mentoring per week, per student. Since we were five student collaborating, we had a combined budget of 5 hours a week. We decided as a group to reach out to two mentors and divided our time between them.

#### Mentor 1
Mentor 1 was the teacher in our Design pattern course. We reached out to him as we were planning a very system heavy project that required a robust core architecture. Knowing his extensive knowledge in this subject, we believed he would be an excellent mentor for us.

#### Mentor 2
Mentor 2 had been available to the class during our introductory C++ course.
Besides being an excellent mentor in that course, he also had significant experience working with Unreal Engine. We felt that his experience with Unreal Engine, and general game development would prove valuable for us.

## Goal Achievement
I am happy to report that all of the broader team goals were achieved for the project, as well as all of my individual ones. A few things from the original plan chaned along the way, with some features being left quite barebone, with some new ones taking their place instead.

#### Team Goals Achieved
Overall, the teams goals we mostly fulfilled. We managed base implementations for eveything we set out to do. Some aspect of the plans weren't fully realized though, while some other features which did recieve more development time, instead ended up more advanced and feature rich than we had planned.

- **Procedural Generation:** Only the basics were implemented, and it could have been more interesting if we had spend more development time expanding upon the item rolling and dungeon creation.

- **Seeded Random:** The system is functional and in use. But since we are not exposing the user to it in any way yet, it is like it wasnt even there.

- **Unit Functionality:** Fully realized as planned. both the player and the AI have the same control over the units, using the same functions to interact with them in the end.

- **Item-Based Abilities:** All of the units' abilities come from the items as planned, as well as some of the stats. Units do have some inherent stats at in the 1.0 build though, but this would be easily changed if prioritized.

- **Networking Support:** Fully implemented with consistent gameplay whather there is one or four playes.

- **Server Authority and MVC:** The server controls everything in the game, with all the clients requesting actions from the server and handling theit own visuals through the Visualization system. 

#### Individual Goals Achieved
Although I had to spend a substantial amount of time searching for an internship, which impacted my ability to achieve all my goals, I was fortunate to realize most of them.

- **Unreal Engine Proficiency:** I now feel so much more comfortable using the Engine than I was going in to the project. Previously, I had mainly worked with and kept to the C++ and had limited interaction with the editor. Without designers and artists on this project, I have spent more time with the engine and it has made me much more proficient and confident using it.

- **Team Collaboration:** Collaboration within the team has been great. I also feel more confident working in a code base where I haven't had a hand in every part of it, and don't know the in and out of everything.

- **Networking:** The networking aspect of the project has both been fun and very challenging. The networking has been the cause of most major issues during the project, but we managed to work though most of them in the end.

- **Game AI Development:** I personally took sole responsibility for developing the AI for the game. I succeded in creating a solid base system and some basic behaviours for it. I am happy with how it turned out, while at the same time wishing that I had more time to work on the system and extending it with more advanced behaviours.

- **C++ and Angelscript Proficiency:** My grasp of C++ and it's quirks, has grown significantly through this project. Focusing on, and spending time working with parts of C++ I didn't fully grasp before, has made me a much better programmer. Additionally, I really appreciate how I managed to use Angelscript for high-level AI behaviours, unit actions and their visualizations.

## Skills and Knowledge Gained

## Challenges and Solutions

## Project Analysis

## Conclusion

## References

