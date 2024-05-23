
# Graduation Project Report, Erik Lund

## Keywords
Engine: Unreal Engine 5, C++, AngelScript by Hazelight, Epic Online Services.
Tools: GitHub, Jira.
Details: 10 weeks, 5 programming students.
Team: Erik Lund (myself), Alice Kamil, Anton Hedlund, Johan Brandt, Theo Sanden. 

## Introduction
This report concerns my ten week long graduation project at FutureGames Stockholm, between Mars 18th and May 24th 2024. During the project I worked on creating a game together with four other programming students at FutureGames.

The personal goals I set for myself upon starting the project were as follows:
Improve my knowledge and gain experience regarding network programming, both in a general sense and specifically in regards to Unreal Engine. I believe this skill to be very valuable to the point of almost being a necessity for a modern game programmer.
Learn about and attempt to create procedural generation of some form. It is a topic that has always intrigued me and a skill I would like to achieve/improve.
Gain a deeper understanding of the dynamics of working with several other programmers. I have experience working in groups with people of other disciplines (artists and designers), but very little experience working with multiple programmers.

## Planning and Starting up
We started out discussing what type of project we wanted to make and decided on making a game project. The plan for the game was to be a tactical rogue-like game with turn-based combat on a grid with some light rpg elements. We also decided that the game should be made for online, co-operative, multiplayer and we wanted to use HazeLights Angelscript when making the game. These last two points came more from a personal interest from the group members wanting to learn and improve in these areas rather than being game design decisions. We then made a rough outline of weekly milestones to have a concrete way of determining if we are keeping pace or falling behind, which looked like this:
Week 1 - Planning and setup
Week 2 - First playable / MVP
Week 3 - Pre-alpha
Week 4 - Alpha build
Week 5 - Pre-beta
Week 6 - Beta build
Week 7 - Content development
Week 8 - Release candidate build
Week 9 - Final polish
Week 10 - Project wrap-up, project presentation

When the overarching plan for the game was in place we created resources to help facilitate and guide us in making a cohesive project. We created a document outlining our agreed upon coding and naming conventions, and a glossary to keep track of what different words or terms meant in regards to the project. We also set up a JIRA board, which helped in defining tasks and assigning them to different members of the team.

## Game Pitch


## Mentors

## Weekly Summary
**Week 1 - Planning & Creating Basic Grid:**
The first week was when most of the planning outlined in the previous section took place. The first couple of days were mostly dedicated to group meetings in order to settle on a project idea and lay a foundation in terms of game design decisions. During the latter half of the week I worked on creating a very basic grid for our game to take place upon and basic units for my teammates to start attaching functionality onto starting next week.

**Week 2 - Inventory:**
The following week I created an actor component for the unit class to hold information about all of that unit’s items, in other words an inventory. The inventory class stored pointers to the individual items in each of the active slots (initially six, later lowered to four), as well as a list of every item (both passive and active) in the unit’s possession. The class itself was not very complex, it was mainly meant to be used as an interface for adding and removing items and keeping all of the item related information in one place with easy methods of getting access to it.

**Week 3 - Improved Units Spawning:**
Up until this point the game only featured simple units with basic movement and attack items. Since I was the one who had worked on creating the grid and spawning units onto it, and also on the inventory component, I took on the responsibility to start improving upon this. I streamlined unit spawning by using a struct to hold any necessary information about the unit, such as items, name and appearance. This struct was passed into the spawning method as a parameter and the values within it were assigned to the unit at the moment of spawning it in.

**Week 4 - Character Select Menu:**
During this week I was tasked with creating a menu preceding the game level where one would be able to select which player controls which unit and also what basic item loadouts each unit would start the game with. This proved a challenge for me as I had not much prior experience with UI in Unreal, and I had no prior experience creating UI which would have to replicate properly for online multiplayer. This lack of experience meant it took most of the week to create this menu, whereas my initial estimation was that it would only take a couple of days. I also had to come back to alter and improve aspects of this menu throughout the remaining weeks of development.

**Week 5 - Procedurally Generated Rooms:**
This week I finally started working on one of my personal main areas of interest, that being procedural generation. With the way our game was designed there was no need for very complex generation logic, and with half of the project already being behind us there was also not enough time for a deep dive into what procedural generation has to offer. I spent the first two days of the week coming up with an idea of how to generate new “rooms” for our game board and then refining the idea further with one of our mentors, Ian Bradley. When I was satisfied with the plan I started implementing the basics of it which, to my surprise, I managed to get into place before the end of the week. The details of the procedural generation will be expanded upon in the next section of this rapport.

**Week 6 - Improvement on previous areas:**
For this week

**Week 7 - Tidying up:**
Similar to last week

**Week 8 - Additional Victory Conditions:**
During early design meetings for this project we had the idea of each room having its own objective the players would need to complete before being able to progress to the next room. This idea had been implemented, however it had been scoped down to only having one and the same objective in each room, that being to defeat all the enemies in that room. With me being the one who worked on creating the rooms and with most of my tasks for the project being completed I found I had the time to expand on this system. With there not being much left of the development time I only managed to implement one additional objective, or victory condition as we named them. The second possible victory condition a room could spawn with was to pick up all the keys in the room (usually one or two) and then get one of the player units to the exit. There would still be enemies in the room that would try to hinder the players but they were not vital to complete the objective. In fact, per a design decision, any enemies still alive when the players managed to complete this type of room would be automatically slain.

**Week 9 - Squashing Bugs:**
When working with other programmers whose systems they’ve designed interconnect with yours there is bound to be unforeseen issues. Therefore the final week of development was dedicated, by group decision, entirely to polishing the game and removing any bugs we managed to find.

**Week 10 - Administrative work, report writing:**
We decided to set the deadline for the actual development of the project to be Friday of week 9. This way we could spend the last week focusing entirely on administrative work such as writing this very report, packaging and uploading the game on public platforms, etc.

## Personal Achievements


## Summary


## Reflections
