# Crazy Podiums

Crazy Podiums is a command-line game where players compete to rearrange animals on two podiums to match a target configuration. Players issue a sequence of commands to manipulate the animals, and the first player to find a correct sequence wins the round.

## Compilation and Execution

### Compilation

To compile the game, you will need a C compiler like `gcc`. Run the following command in your terminal:

```bash
gcc -o crazy_podiums *.c -std=c99 -Wall
```

### Execution

To start the game, run the compiled executable from your terminal, followed by the names of the players (at least two).

```bash
./crazy_podiums Player1 Player2
```

## Game Rules

### Objective

The goal of the game is to be the player with the highest score at the end of the game. A round is won by finding the correct sequence of commands to transform the current arrangement of animals on the podiums into the target arrangement.

### How to Play

1.  At the start of each round, the game displays the current state of the two podiums (Blue and Rouge) and the target state.
2.  Players take turns entering a command sequence to try and reach the target state. The input format is: `PlayerName COMMAND_SEQUENCE` (e.g., `Player1 KILOSO`).
3.  The available commands are:
    *   `KI`: Moves the top animal from the Blue podium to the top of the Rouge podium.
    *   `LO`: Moves the top animal from the Rouge podium to the top of the Blue podium.
    *   `SO`: Swaps the top animals of the Blue and Rouge podiums.
    *   `NI`: Moves the bottom animal of the Blue podium to the top of the Blue podium.
    *   `MA`: Moves the bottom animal of the Rouge podium to the top of the Rouge podium.
4.  If a player's sequence is correct, they win the round and score one point. A new round begins.
5.  If a player's sequence is incorrect or invalid, they are eliminated from the current round.
6.  The round continues until a player finds the correct sequence. If all but one player are eliminated, the last remaining player automatically wins the round.
7.  The game ends when all target configurations have been successfully completed or when a player types `QUIT`.

### Scoring

*   The player who successfully completes the objective of a round gets one point.
*   At the end of the game, the players are ranked based on their total scores.

## Configuration File

The game's behavior can be customized through the `crazy.cfg` file.

The `crazy.cfg` file has two lines:

1.  **Line 1:** A space-separated list of animal names to be used in the game.
2.  **Line 2:** A space-separated list of commands that are allowed to be used.

### Example `crazy.cfg`

```
OURS ELEPHANT
KI LO SO NI MA
```
