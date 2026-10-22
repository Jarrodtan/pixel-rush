# Pixel Rush

A 2-player local co-op 2D platformer made by team *Pixel Jump* for CSD1451 at DigiPen Institute of Technology Singapore (SIT-DigiPen), built in C++ on DigiPen's Alpha Engine.

One player controls the **Light** character and the other controls the **Shadow** character. Each player has to collect their own shards (light shards or shadow fragments) scattered around the level, and both have to finish to clear the stage.

<!-- Screenshots / gameplay GIF go here -->

## Features

- Local 2-player co-op on one keyboard, with double jump
- Tile-based levels loaded from map files
- Health bars, power-ups, particles and an in-game stats overlay
- Game state manager (splash, main menu, tutorial, level, win/lose, credits)
- FMOD audio for background music and sound effects

## My role

- **Mesh creation**: the shared mesh-building code that every game object is rendered with ([`MeshCreation.cpp`](pixel_jump/MeshCreation.cpp))
- **Level game state**: parts of the main level logic and the game state list
- **Credits screen**
- Smaller contributions to the camera, game objects and rendering helpers

## Tech stack

C++ · DigiPen Alpha Engine · FMOD · Visual Studio

## Building (Windows)

1. Open `pixel_jump.sln` in Visual Studio 2022.
2. Build and run with **F5**. The Alpha Engine and FMOD libraries are included in `Extern/`.

## Controls

| Light player | Shadow player | Action |
| --- | --- | --- |
| A / D | ← / → | Move |
| W (double-tap for double jump) | ↑ (double-tap) | Jump |

Esc returns to the main menu, and T toggles the stats display.

## Team

Lau Xiang Yang, **Tan Wei Leong**, Ng Wei Zhen, Mira Tristan Dumlao

---

Made as a student project at DigiPen Institute of Technology Singapore. All content © 2025 DigiPen Institute of Technology Singapore.
