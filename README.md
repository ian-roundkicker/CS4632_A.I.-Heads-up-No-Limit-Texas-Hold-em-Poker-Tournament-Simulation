# CS4632_A.I.-Heads-up-No-Limit-Texas-Hold-em-Poker-Tournament-Simulation
This project is a simulation of Heads Up No Limit Texas Hold'Em poker allowing for you to run games of custom Poker Bots against each other.

# Installation instructions
Option 1: Visual Studio
First, clone the repository.
Second, clone this repository and follow the instructions to generate the .lib files: https://github.com/HenryRLee/PokerHandEvaluator/tree/develop/cpp
Make sure that you use CMake to select the generator option for Visual Studio.
After this, go to the Visual Studio Project Property Pages for this project. Under the C/C++ section there is a section called Additional Include Directories. Edit this to the path of the include directory under the PokerHandEvaluator's cpp/include
After this, go to the Linker section of the Property Pages, and change the section that is titled "Additional Library Directories" to the path of cpp\build\Release, or whatever folder you generated the .lib files in.
Option 2: GCC
Download the .cpp file in the directory. Clone the GitHub repository: https://github.com/HenryRLee/PokerHandEvaluator/tree/develop/cpp
Follow the instructions to generate the .a files.
Place the .h files in the same directory as the .cpp file (and perhaps edit the \#include directive properly attach to the path). Utilize the command prompt to link the .a files.

#  Dependencies and versions
The only dependency is HenryRLee's Poker Evaluator. I suspect that the .h files are the only requirement at the moment, but that could change by next iteration.

# Troubleshooting common issues
The additional library directories and include directories use paths from my own computer. They will have to be changed in order for the program to run.
If you are unable to use CMake to generate Visual Studio .lib files, check if your version of CMake is up-to-date.

# Usage
Currently the user is unable to set parameters without altering code. You run the simulation by running it in Visual Studio (or compiling it with GCC).

# Configuration
configuration.json has the following parameters:
  "num_games" - number of games before program stops. Each game will continue until one player is out of chips
  "num_chips" - starting number of chips for both players
  "append_to_db" - true to append to current database, false to delete current database
  "Bot1" - consult PlayerHand.cpp to see what behaviors there are. Set the value to one from the switch statement
  "Bot2" - same as bot1

# Expected output/behavior
The simulation runs until only one player has chips. If a variable in a for loop at the bottom of the main method is changed, more than one game may be played.
Player 1's hand:
3 of Diamonds
4 of Diamonds

Player 2's hand:
3 of Clubs
9 of Clubs

Community card 1: Ace of Hearts
Community card 2: Ace of Clubs
Community card 3: 8 of Clubs
Community card 4: 7 of Spades
Community card 5: Judge of Diamonds
Player 2 was at rank 1
Player 1 was at rank 2
Player 2 won the hand and now has 9 chips.

Player 1's hand:
8 of Diamonds
Queen of Clubs

Player 2's hand:
2 of Diamonds
3 of Diamonds

Community card 1: 2 of Spades
Community card 2: King of Diamonds
Community card 3: 6 of Hearts
Community card 4: Queen of Hearts
Community card 5: 3 of Clubs
Player 2 was at rank 1
Player 1 was at rank 2
Player 2 won the hand and now has 10 chips.
END OF PROGRAM

# Architecture Overview
Here is a list of the components:
Card: Represents one card (uses an integer)
Game: Manages every aspect of the game
PlayerHand: Represents a player
AgentBehavior: Abstract Class all bots inherit from

No other component has been added yet. I may do a Commit later to specify details on the functions.
My original UML design was very rushed. I made one major deviation from my initial concept: PlayerHand (Player) is a class and not an interface. In addition, any calls to A.I models will be done with a function either passed in the constructor or modified with a method.
