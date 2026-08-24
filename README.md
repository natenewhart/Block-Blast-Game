# Block Blast Game - Work In Progress...
This is a simple recreation of the Block Blast game, built using C++ and the SFML library.

## Features
* 8x8 grid where you drag and drop blocks to place them
* 16 different block shapes
* Blocks snap to the nearest open spot on the grid as you drag them near it
* Full rows and columns clear automatically once they're completely filled
* A hand of 3 blocks is dealt at a time. Once you've placed all 3, a new hand is generated
* The hand generator checks that every block it deals can actually be placed somewhere on the current board, so you won't get stuck with an unplaceable block

## Getting Started
### Prerequisites
Make sure you have visual studio 2022 or later installed, along with the C++ packages.
the code uses `<print>`, requiring a C++23-capable toolchain (MSVC in VS2026 with `/std:c++latest` or newer).
C++

### Installation / Building

1. Clone the project into visual studio and build to compile, the project file is already set up with all links and dependencies.
> Note: The SFML library comes pre-installed in the \external folder.
3. Make sure `res/cour.ttf` exists relative to the working directory.
4. Build and run (`main.cpp` is the entry point`).

## Controls
* **Left click and hold** on a block in your hand to pick it up
* **Drag** the block around the grid, it'll show a preview of where it will land and highlight any rows or columns it's about to clear
* **Release** over a valid spot to place the block
* **Release** over an invalid spot and the block will snap back to its tray position
* **Esc** to close the game

## Gameplay
<img width="1282" height="752" alt="Screenshot 2026-08-23 192920" src="https://github.com/user-attachments/assets/01b74dba-c9c1-4acb-9fbf-30a0f54f0dac" />
<img width="1282" height="752" alt="Screenshot 2026-08-23 192910" src="https://github.com/user-attachments/assets/7a0ad9e1-997c-4453-88dd-513577af6d9e" />
<img width="1920" height="1152" alt="Screenshot 2026-08-23 192837" src="https://github.com/user-attachments/assets/8c1379b6-5a70-422d-8c17-f9fea3de8088" />


## License
COMING SOON...
