# Othello Game

A console-based implementation of the classic Othello (Reversi) board game written in C++.

## Description

This is a two-player Othello game that runs in the terminal. The game features a standard 8x8 board with the traditional starting position. Players take turns placing pieces (Black 'B' and White 'W') on the board.

## Features

- **Interactive Gameplay**: Command-line interface for easy interaction
- **Save/Load Game**: Save your current game state and load it later
- **New Game**: Start a fresh game at any time
- **Turn-based Play**: Alternating turns between Black (B) and White (W) players
- **Board Display**: Clear visual representation of the current board state

## How to Compile and Run

### Prerequisites
- C++ compiler (g++, clang++, or MSVC)
- Standard C++ library support

### Compilation
```bash
g++ -o othello main.cpp
```

### Running the Game
```bash
./othello
```

On Windows:
```cmd
othello.exe
```

## Game Rules

Othello is played on an 8x8 board. The game starts with four pieces in the center:
```
  A B C D E F G H
1 . . . . . . . .
2 . . . . . . . .
3 . . . . . . . .
4 . . . W B . . .
5 . . . B W . . .
6 . . . . . . . .
7 . . . . . . . .
8 . . . . . . . .
```

- Black (B) always goes first
- Players must place pieces adjacent to existing pieces
- The goal is to have the most pieces of your color on the board

## Available Commands

| Command | Description | Example |
|---------|-------------|---------|
| `new` | Start a new game | `new` |
| `place <position>` | Place a piece at the specified position | `place D4` |
| `save <filename>` | Save the current game state | `save mygame.oth` |
| `load <filename>` | Load a previously saved game | `load mygame.oth` |
| `exit` | Exit the game | `exit` |

### Position Format
Positions are specified using chess notation:
- Columns: A-H (left to right)
- Rows: 1-8 (top to bottom)
- Example: `D4` refers to column D, row 4

## Game Flow

1. The game automatically starts a new game when launched
2. The current board state and whose turn it is will be displayed
3. Enter commands at the `> ` prompt
4. After each valid move, the board updates and switches to the next player
5. Use `save` to preserve your game progress
6. Use `load` to continue a previously saved game
7. Type `exit` to quit the game

## File Structure

```
ap_phase1/
├── main.cpp          # Main game implementation
├── README.md         # This file
└── .gitignore        # Git ignore file
```

## Classes Overview

- **Board**: Manages the 8x8 game board, piece placement, and board state
- **Player**: Represents a player with their color (B or W)
- **Game**: Controls game flow, turn management, and user interaction

## Example Gameplay

```
New game started
  A B C D E F G H
1 . . . . . . . .
2 . . . . . . . .
3 . . . . . . . .
4 . . . W B . . .
5 . . . B W . . .
6 . . . . . . . .
7 . . . . . . . .
8 . . . . . . . .
Player Turn: B
> place D3
Piece placed at D3
  A B C D E F G H
1 . . . . . . . .
2 . . . . . . . .
3 . . . B . . . .
4 . . . W B . . .
5 . . . B W . . .
6 . . . . . . . .
7 . . . . . . . .
8 . . . . . . . .
Player Turn: W
>
```

## License

This project is for educational purposes.