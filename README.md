# 2D Platformer Game (C++ using SGG)
This is a 2D platformer game made in C++ using the [Simple Graphics Library (SGG)](https://github.com/cgaueb/sgg).  
The player controls a character who moves through levels, avoids enemies, collects items, and tries to reach the goal. The game was created for a university project (AUEB) and focuses on both gameplay and good programming practices.


## Technologies Used

- **C++17** – Main programming language
- **SGG (Simple Graphics Library)** – Handles graphics, input, and audio

## Goals
### Required features set by the assignment:

- **SGG Integration**  
  All graphics, input handling, and audio are implemented using only the Simple Graphics Library (SGG), as required. No other external libraries are used.

- **Dynamic Memory Usage**  
  Game entities such as enemies, projectiles, and temporary effects (e.g., particles) are created using `new` and properly destroyed when no longer needed.

- **Object-Oriented Design with Polymorphism**  
  A full hierarchy of game objects is implemented, based on a base `GameObject` class that includes `update()`, `draw()`, and `init()` methods. These methods are overridden in derived classes (e.g., `Player`, `Enemy`, etc.).

- **Centralized Management**  
  Appropriate Singleton classes [UIManager](C_Plus_Plus_Game\UIManager.cpp), [ParticleManager](C_Plus_Plus_Game\ParticleManager.cpp), [MusicManager](C_Plus_Plus_Game\MusicManager.cpp), [LevelManager](C_Plus_Plus_Game\LevelManager.cpp),  manage global game data such as: levels, score, transitions, and object access.

- **STL Collections**  
  STL containers like `std::vector` and `std::map` are used to store and manage game entities, UI components, and levels. 

- **Collision Detection**  
  Bounding box collision detection is implemented for interactions between the player, walls, enemies, and pick-up items.

### Optional Features and Code Quality set by the assignment:

- Clear and organized code, split into different files, folders and classes for better structure.
- Use of `const` in functions and variables where values don’t change.
- Use of references instead of copies to avoid unnecessary overhead.
- Templates used in functions or classes where needed ([impl1](C_Plus_Plus_Game\CstmCallback.h#L10), [impl2](C_Plus_Plus_Game\Manager.h), [impl3](C_Plus_Plus_Game\Entity.h#L10)).
- Multithreading, especially for heavy calculations (e.g. AI or effects) ([impl1](C_Plus_Plus_Game\ParticleManager.cpp#L13), [impl2](C_Plus_Plus_Game\Level.cpp#L64)).
- [Levels loaded from files](C_Plus_Plus_Game\Level.cpp#L70), so the game can support many stages.
- Basic event system ([GameEvents](C_Plus_Plus_Game\GameEvents.h), [Callbacks](C_Plus_Plus_Game/CstmCallback.h)), allowing for decoupled communication between game objects.
- Temporary effects (e.g. point popups, smoke, transitions) are created and removed properly during gameplay.

## Credits

[See full asset credits here](Credits.md)