# Block Blast Game
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
2. Build and run (`main.cpp` is the entry point`).

## Controls
* **Left click and hold** on a block in your hand to pick it up
* **Drag** the block around the grid, it'll show a preview of its position and highlight any rows or columns it will clear upon placement
* **Release** left click on grid to place block
* **Release** over an invalid spot and the block will snap back to its tray position
* **Esc** to pause the game

## Gameplay
https://github.com/user-attachments/assets/2461afec-484a-4c51-94d3-54a34a2fda87

Photos:
<img height="15%" alt="Screenshot 2026-10-13 192911" src="https://github.com/user-attachments/assets/14c0c925-d30f-46bf-a19a-ece88eed1f75" />
<img height="15%" alt="Screenshot 2026-10-13 192926" src="https://github.com/user-attachments/assets/007e39b7-81c8-4238-853f-b501d102bbd3" />
<img height="15%" alt="Screenshot 2026-10-13 192948" src="https://github.com/user-attachments/assets/0a345739-a289-4e46-b02f-ccca4ec31c12" />

## License
COMING SOON...
