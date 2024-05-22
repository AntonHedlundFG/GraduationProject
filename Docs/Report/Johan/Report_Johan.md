# Graduation Project Report, Johan Brandt

## Keywords
**Engine:** Unreal Engine 5, C++, AngelScript by HazeLight, Epic Online Services.
\
**Teamwork:** GitHub, Jira.
\
**Details:** 10 weeks, 5 programmers.
\
**Concepts:** Command Pattern, Model-View-Controller (MVC), Strategy Pattern, Utility AI, Networking, Multiplayer, Procedural Generation

## Table of Contents
1. [Introduction](#introduction)
   - [Project Scope and Significance](#project-scope-and-significance)
   - [Individual Project Goals](#individual-project-goals)
   - [Team Project Goals](#team-project-goals)
   - [Early Design](#early-design)
   - [Anticipated Challenges](#anticipated-challenges)
   - [Style Guide](#style-guide)

2. [Work Process](#work-process)
   - [Initial Week-by-week Plan](#initial-week-by-week-plan)
   - [Project Management and Communication](#project-management-and-communication)
   - [Mentorship and Guidance](#mentorship-and-guidance)

3. [Goal Achievement](#goal-achievement)
   - [Team Goals Achieved](#team-goals-achieved)
   - [Individual Goals Achieved](#individual-goals-achieved)

4. [Skills and Knowledge Gained](#skills-and-knowledge-gained)
   - [Game AI Development](#game-ai-development)
   - [System Design Architecture](#system-design-architecture)

5. [Challenges and Solutions](#challenges-and-solutions)
   - [Network Development](#network-development)
   - [Debugging Multiplayer](#debugging-multiplayer)
   - [Reworks upon Reworks](#reworks-upon-reworks)

6. [Project Analysis and Conclusion](#project-analysis-and-Conclusion)

7. [References](#references)


## Introduction
This is the report covers my graduation project at Futuregames in Stockholm, undertaken from March 18 to May 24. This project offered an opportunity to explore and practice with the capabilities of Unreal Engine while integrating it with Hazelight's AngelScript. 
Through this report, I will detail the process, challenges, and learning outcomes of my graduation project.

The project was made as a team effort with five programming students:\
[Me](https://www.johanbrandt.com/), [Anton Hedlund](https://www.antonhedlund.com/), [Erik Lund](https://lunderik.wixsite.com/portfolio), , [Alice Kamil](https://alicekamil.com/), [Theo Sandén](https://www.theosanden.com/).

### Project scope and Significance
For this project, I have worked in a team of five programmers to develop a turn-based tactical rogue-lite game with optional cooperative multiplayer features over a network session. \
Online session management was handles using Epic Online Services for Unreal Engine. We utilized a modified version of Unreal Engine 5.3.0, which included support for Hazelight's AngelScript, to build the game.

### Individual Project Goals
My primary goals for the project were as follows:

- **Enhance proficiency with Unreal Engine:** To practice and advance my familiarity with Unreal Engine.
- **C++ and Scripting Integration:** To practice and advance my skills in C++ along with a scripting language, Angelscript in this case. 
- **Team Collaboration:** To gain experience working with a larger programmer team of five members, compared to my previous experience with a maximum of three. 
- **Networking in Games:** To develop more hands-on experience working with  networking in games. 
- **AI System Development:** To build and develop a robust base for the game's AI that would support all the possible behaviours needed for the game, both now and in future iterations of the project.

### Team Project Goals
Our goal was to create a tactical, almost board game-like experience, influenced by games like Descent, Gloomhaven and Warhammer: Quest. We also aimed to incorporate elements from procedurally generated roguelite games such as The Binding of Isaac, FTL: Faster Than Light, and Hades.

The key pillars for the game that we agreed upon were:
- **Procedural generation:** To implement procedural generation major systems in the game such as loot and the game grid. 
- **Seeded Random:** A seeded approach to the randomness in the game ensuring repeatable and consistent outcomes.
- **Command-pattern:** To use the command-pattern for game actions, allowing players to redo all the actions that they have made..
- **Functionally Identical Units:** Ensuring that the units function identically for both players and AI. The AI should interact with the game exactly as the player does, with access to nor more or less information than the player has.
- **No Inherent Abilities:** Units have no inherent abilities and stats; All abilities and stats come from the items.
- **Networked Multiplayer:** Support for up to four players over a network session.
- **Server Authoritative Control:** Server authoritative control where all actions and events happens on the server, with clients requesting actions.
- **Model-view-controller:** Using the MVC pattern for visuals, where clients handle their own visual information.
- **Gameplay Agnostic to Player Count:** Ensuring gameplay is agnostic to the number of players, with fewer players controlling more units.

### Early design
The game was designed to operate on a square grid where players take turns with AI to control character units. Players shared control over four characters, with each player able to manage multiple characters if there were fewer than four players. All units' stats and abilities were to be granted by items, leaving no innate abilities for the units except for the ability to use items. Items could be slot-specific, granting abilities, or slot-less, providing passive effects. For example, boots might grant a movement ability, a weapon might grant an attack ability, and armor could increase health value. Slot-less items could instead grant passive increases to, for example movement range or attack.

Enemy units were designed to be identical to player-controlled units, differing only in the controlling entity, which simplified integration and interaction within the game environment. It would also give a sense of fairness to the game and inherently show the player how the AI controlled units would be able to act.

Planning Whiteboard 1 | Planning Whiteboard 2 | Planning Whiteboard 3
:----:|:----:|:----:
![PlanningWhiteboard1](https://github.com/AntonHedlundFG/GraduationProject/blob/main/Images/Planning_Whiteboard_cropped.png) | ![PlanningWhiteboard2](https://github.com/AntonHedlundFG/GraduationProject/blob/main/Images/Planning_Whiteboard2_cropped.png) | ![PlanningWhiteboard3](https://github.com/AntonHedlundFG/GraduationProject/blob/main/Images/Planning_Whiteboard3_cropped.png)

Given the absence of dedicated artists, we adopted a minimalistic art approach using basic geometric shapes and billboards for the game actors. Additionally, we tried to emphasize the use of scalable and extendable system architecture. The plan was to use this approach to content generation as much as possible, given the lack of dedicated designers. The goal was to create a lot of content using systems instead of creating it by hand.

### Anticipated Challenges
One of the largest challenges I anticipated was to come from working in a team of five programmers without the support of dedicated artists or designers. Previously, I had worked with a maximum of three other programmers, and even in that smaller group, task allocation and collaboration often presented difficulties. Organizing our team to avoid these issues as much as possible would be crucial.\
Additionally, the absence of designers meant that we needed to architect our systems to be scalable and easily extendable. The systems had to be robust from the start, allowing for efficient content addition throughout the project and potential expansion later on. This required careful planning and systematic content generation, which would be a key focus as the project unfolded.

### Style guide
During the planning phase of the project, we established specific [Naming Conventions](https://github.com/AntonHedlundFG/GraduationProject/blob/main/Docs/Naming%20Conventions/README.md). Some of these conventions are standard, while others are unique to our project.
For example, while following the default Unreal naming conventions, we also agreed on additional rules: all native C++ classes would start with a "C" (e.g. CMyActor), and all AngelScript classes would start with an "S" (e.g. SMyActor). 

## Work Process

### Initial Week-by-week Plan
In the initial planning of the project, we laid out a flexible plan with milestone for each week. The plan served as a guide rather than a fixed schedule, allowing us to adapt as needed throughout the project.

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
The team will use Discord for online communication, GitHub for version control and Jira for task management.\
We will have a stand-up meeting, daily, at 10 AM. We won't have weekly reviews of the project, but we will make sure to have a weekly build from week 2 and onwards. Review meetings for the whole team will be planned and scheduled for when there is a need.

### Mentorship and Guidance
The school provided us with a budget of 1 hour or mentoring per week, per student. Since we were five student collaborating, we had a combined budget of 5 hours a week. We decided as a group to reach out to two mentors and divided our time between them.
#### Marc Zaku
Mentor 1 was the teacher in our Design pattern course. We reached out to him as we were planning a very system heavy project that required a robust core architecture. Knowing his extensive knowledge in this subject, we believed he would be an excellent mentor for us.
#### Ian Bradley
Mentor 2 had been available to the class during our introductory C++ course.
Besides being an excellent mentor in that course, he also had significant experience working with Unreal Engine. We felt that his experience with Unreal Engine, and general game development would prove valuable for us.

## Goal Achievement
I am happy to report that all the broader team goals were achieved for the project, as well as all my individual ones. A few things from the original plan changed along the way, with some features being left quite barebone, with some new ones taking their place instead.

### Team Goals Achieved
Overall, the teams’ goals we mostly fulfilled. We managed base implementations for everything we set out to do. Some aspects of the plans weren't fully realized though, while some other features which did receive more development time, instead ended up more advanced and feature rich than we had planned.

- **Procedural Generation:** Only the basics were implemented, and it could have been more interesting if we had spend more development time expanding upon the item rolling and dungeon creation.

- **Seeded Random:** The system is functional and in use. But since we are not exposing the user to the seed in any way yet, it is like it wasn't even there.

- **Unit Functionality:** Fully realized as planned. both the player and the AI have the same control over the units, using the same functions to interact with them in the end.

- **Item-Based Abilities:** All of the units' abilities come from the items as planned, as well as some of the stats. Units do have some inherent stats at in the 1.0 build though, but this would be easily changed if prioritized.

- **Networking Support:** Fully implemented with consistent gameplay whether there is one or four players.

- **Server Authority and MVC:** The server controls everything in the game, with all the clients requesting actions from the server and handling their own visuals through the Visualization system. 

### Individual Goals Achieved
Although I had to spend a substantial amount of time searching for an internship, which impacted my ability to achieve all my goals, I was fortunate to realize most of them.

- **Unreal Engine Proficiency:** I now feel so much more comfortable using the Engine than I was going in to the project. Previously, I had mainly worked with and kept to the C++ and had limited interaction with the editor. Without designers and artists on this project, I have spent more time with the engine, and it has made me much more proficient and confident using it.

- **Team Collaboration:** Collaboration within the team has been great. I also feel more confident working in a code base where I haven't had a hand in every part of it, and don't know the in and out of everything.

- **Networking:** The networking aspect of the project has both been fun and very challenging. The networking has been the cause of most major issues during the project, but we managed to work though most of them in the end.

- **Game AI Development:** I personally took sole responsibility for developing the AI for the game. I succeeded in creating a solid base system and some basic behaviours for it. I am happy with how it turned out, while at the same time wishing that I had more time to work on the system and extending it with more advanced behaviours.

- **C++ and Angelscript Proficiency:** My grasp of C++ and it's quirks, has grown significantly through this project. Focusing on, and spending time working with parts of C++ I didn't fully grasp before, has made me a much better programmer. Additionally, I really appreciate how I managed to use Angelscript for high-level AI behaviours, unit actions and their visualizations.

## Skills and Knowledge Gained
Outside of and on top of the goals I set out for the project I also learned quite a bit about a few other subjects.

### Game AI Development  
I took responsibility for the AI in the game, and I ended up as the sole programmer and implementer of the system for the game. Given that we were developing a game with a wide variety of actions and items, I aimed to develop and implement a robust AI system capable of handling all this complexity. After researching for a few days, I ended up deciding in on implementing Utility AI. 

Utility AI, or Utility-based AI, is a system designed to assign utility scores to various potential actions for the AI can take. The scores represent the usefulness of each action in the current context which the AI is in. The AI then selects the action with the highest utility score. My system also tries to recursively to this and plan out each round of actions before starting the process of executing them. 

In addition to Utility AI, I also explored Goal-Oriented Action Planning (GOAP). I set up a plan for how to integrate GOAP into the AI system to manage overall team behaviours through updating of the contexts, time constraints during the project prevented me from fully implementing it. Nonetheless, I am quite happy with the foundational framework I made for the AI as it barely took any work to make it functional with all the new items and abilities that we threw at it towards the end of the project.

### System Design Architecture 
Going into the project we all knew it would be a systems-heavy endeavour, and we tried planning for it accordingly. Although learning and practicing specific design pattens wasn't a primary goal for me going into this project, I still feel much more confident in a few major ones that we implemented.

We introduced and implemented three major pattens into the core design of the project: the **Command Pattern**, the **Model-View-Controller Pattern (MCV)**, and the **Strategy Pattern**.

- **Command-Pattern:** We implemented the command pattern as the underlying structure for all the Actions in the game. This design choice was to ensure that all actions were encapsulated as object, making them more easily manageable and undoable. The encapsulation also allowed for easier replication to clients, for client-side visualization.
- **Model-View-Controller (MVC):** We used the MVC pattern to visualize actions on all clients in our game. The process works as follows: the client requests an action to be performed on the server; the server then processes the action before notifying the all the clients of the result; finally, each client updates their visuals through the visualization system based on the server's notification, ensuring that all players see the same result of the action.
- **Strategy Pattern:** The visualisation system implements the strategy pattern. Each visualization includes both a visualization to be played on the client and a list of all actions that would use this specific visualization. After an action is performed on the server it is then queued into the visualization system and replicated to all clients. As long as there are actions queued, the visualizer loops through them and performs the appropriate visualization for each action.

While neither of these patterns were completely new to me, I had never used them extensively in any of my previous project. Although I wasn't directly involved in the actual implementation of these patterns in this project, I still learned a lot from studying and understanding their implementation and using them throughout this project.  

## Challenges and Solutions

### Network Development
Developing a game with network replication is challenging, especially when you have limited experience with it. The difficulty multiplies when you're working as part of a team where everyone is learning and experimenting with new techniques.\
All throughout development we faced many issues and bugs related to networking and server-to-client replication. Often, these problems arose because we primarily tested in a single-player environment, as it was easier and quicker. However, our inexperience also played a significant role, leading us to occasionally forget to replicate the correct properties, or even replicate them at all. 

While we didn't just magically "solve" networking, as there is no single solution to all networking issues, I believe that we managed to address most of the specific problems that arose during the project. Many of our issues were simply remedied by just making sure to replicate the correct properties and testing for as many scenarios as we could think of.

One persistent issue that we never managed to solve completely involved the replication of a collection of pointers. The problem arose from when the collection was replicated before the object themselves had time to properly replicate. For example, when replicating a `TArray<UObjects*>` and using an `OnRep` function that relies on the values in the array, we often encountered `nullptr` instead of the objects. We believe that this issue stems from the `TArray` replicating properly before the objects within the array has had the time to properly replicate, resulting in an array of null pointers on the client side when the `OnRep` function gets called. A workaround that we used to skirt around the issue was to perform null checks through the array on the client, but this solution feels clunky. I believe that there is a better solution, but we have not yet found it.  

### Debugging Multiplayer
Debugging a finding solutions for issues that arose when testing the game in a multiplayer session presented another set of challenges. The standard IDE debugger could not attach to the game instances when we were testing it in Standalone mode, which unfortunately is also required to test networking features properly.

To help solve this problem, we relied on the Logging System I developed early in the project. We used it identify difference in replication and pinpoint the crash or issue arose. While this is far from a perfect solution, it was helpful. I imagine that there are better and more accessible solutions for debugging the online mode, but we were not aware of them at the time. In a larger and more extensive project, I would spend more time on research and ensure that we establish a more comprehensive method of debugging the online features.

### Reworks upon Reworks
We started this project by gathering around a whiteboard to plan out the construction of all the underlying systems for the game. From there we created the first iteration of the underlying command system within a week from writing the first line of code. Although it was not final in any way, it provided a foundation to start working with, and we quickly spread out and to start working on different parts of the project. 

No more than one week later, a part of the team began reworking the core command system into a more extensive system for actions and abilities. By this point, I had started working on the AI, so I was not part of this group. However, as a core system was being changed, I had to pause my work temporarily until the rework was done. This was of course frustrating, even though the changes to the system were beneficial to the project in the end.

Seven weeks into the project, we had yet another overhaul and rework of core systems. This time introducing attributes into the previously reworked ability system. This once again temporarily hampered progress on other parts of the game while it was being implemented. Additionally, existing content had to be updated to support the new attribute system.\
We had learned from the previous rework and the process was made less obstructing through branching of the project. While not a perfect solution or execution, it helped making the impact on other parts less of an issue. I believe that it could have been done even more effective. With proper testing and feature implementation in the branch before merging it into the main branch we could have avoided even more of the obstruction in the end.

The whole process of reworking existing systems in a short project like this one is something that I believe we could have avoided completely. We tried to minimize this through planning and documentation early in the project, but since we made two substantial reworks through a nine-week project, it's clear that we still could have done better job.\
Experience is key here though. The core structures of projects like these are complicated. With more experience, I believe that some of the issues we faced could have been planned for and avoided. I have learned a lot from the project, and while I don't believe that the next project will be perfect either, I believe that I will be able to avoid some of the pitfalls we faced during this project.
 
## Project Analysis and Conclusion
I am very happy to have teamed up with the other four people during this project. We aimed to develop a turn-based tactical rogue-lite game with quite a few complex features during this graduation project. Reflecting on the journey, I believe that the project has been a significant success for both me and the team. Despite the challenged we encountered, we achieved most of our primary goals. The knowledge and skills that I have gained, especially in system design and AI development, will be invaluable for future projects.

The one area that I wish I could have devoted more time to is the AI system. Unfortunately, my search for an internship, which would follow this project, consumed nearly half of my available working hours, limiting the time I could dedicate to it.  During the later parts of this project, I spent my time more as support for the others instead of expanding upon my own work, as I had to dedicate more and more time to the search. Given more time, I would have loved to expand and enhance the AI beyond the foundational work that I managed to complete. Nevertheless, the experience has been incredibly rewarding and fun!

## References
**Utility AI:** \
https://www.gameaipro.com/GameAIPro/GameAIPro_Chapter09_An_Introduction_to_Utility_Theory.pdf \
https://shaggydev.com/2023/04/19/utility-ai/ \
https://www.gdcvault.com/play/1012410/Improving-AI-Decision-Modeling-Through \
https://www.gdcvault.com/play/1021848/Building-a-Better-Centaur-AI 