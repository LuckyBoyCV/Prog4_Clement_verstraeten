[LuckyBoyCV/Prog4_Clement_verstraeten.git](https://github.com/LuckyBoyCV/Prog4_Clement_verstraeten.git)

# Prog 4 Exam Project: Q*bert

This repository contains my Programming 4 exam project: a C++ remake of **Q*bert** built on top of the Minigin framework.

The goal of the game is to jump over all pyramid tiles and change them to the target color while avoiding enemies. The project includes single player, co-op, versus mode, level progression, score/lives UI, high scores, sound, keyboard input, and controller support.

## Features

- Q*bert pyramid gameplay with tile color changes.
- Three game modes:
  - Single Player
  - Co-op
  - Versus, where player 2 controls Coily
- Data-driven level rules from `Data/levels.json`.
- Round and level progression.
- Score, lives, round, and level displays.
- Game over screen with initials entry and high-score list.
- Keyboard and controller support.
- Web/Emscripten build support from the original Minigin setup.

## Enemies

The game contains the main Q*bert enemies and hazards:

- **Coily**  
  Starts as an egg, hatches into a snake, and chases Q*bert. In versus mode Coily can be controlled by player 2.

- **Red Ball**  
  Drops down the pyramid and kills Q*bert on contact.

- **Slick and Sam**  
  Move down the pyramid and reverse tile progress. Q*bert can stop them by landing on the same tile.

- **Ugg and Wrongway**  
  Climb along the sides of the pyramid and act as hazards.

Enemies are spawned and rotated by `EnemySpawnerComponent`. More enemy types become active in later rounds, so the difficulty increases as the player progresses.

## Patterns Used

This project uses several game programming patterns:

- **Component Pattern**  
  Game objects are built from components such as render, text, Q*bert, enemy, HUD, and game-state components.

- **Command Pattern**  
  Keyboard and controller input are mapped to command objects, such as jump commands, menu commands, skip-level commands, and score-entry commands.

- **State Pattern**  
  Q*bert and the enemies use states for behavior like idle, jumping, falling, respawning, moving, egg mode, snake mode, and player-controlled Coily.

- **Observer Pattern**  
  Subjects notify observers when game events happen. This is used for score updates, lives updates, enemy reactions, round changes, level changes, player death, and game over.

- **Service Locator Pattern**  
  The sound system is accessed through a service locator, with a null sound system fallback.

- **Singleton Pattern**  
  Engine-wide systems such as the scene manager, resource manager, input manager, and renderer use singleton access.

## Controls

In the menu, use the arrow keys or controller 0 D-pad to move through the options. Press Enter or the controller A button to confirm.

In single player, player 1 controls Q*bert with the arrow keys. Up jumps up-left, Right jumps up-right, Down jumps down-right, and Left jumps down-left. Controller 0 can also be used with the D-pad.

In co-op, player 1 uses the arrow keys or controller 0. Player 2 uses WASD or controller 1. W jumps up-left, D jumps up-right, S jumps down-right, and A jumps down-left.

In versus mode, player 1 controls Q*bert with the normal player 1 controls. Player 2 controls Coily after it hatches, using WASD or controller 1.

On the high-score entry screen, Up and Down change the selected letter, Left and Right move between the initials slots, and Enter submits the score. Controller 0 can also do this with the D-pad and A button.

F1 can be used as a debug shortcut to skip to the next level.
