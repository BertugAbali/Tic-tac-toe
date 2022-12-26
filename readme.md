**IAPG PL 2022/2023 software project**

List of requirements:
1. Menu system (implemented).   (Implemented/Partially Implemented/Not Implemented): Optional short descriptive sentence with functions used for this requirement
2. Data structure to store each player's data (implemented). In fileSettings.c script, player data saved by updatePlayer, createPlayer, writeTextFile functions.   
3. The tic-tac-toe game (implemented). In game.c script, game playable with controlOfGame, designerOfGame, playerMove, checkTheGame functions.
4. Data structure that allows saving the current state of the board (implemented).
5. Printing the current state of the board on the screen (implemented). In game.c script, the game printing with designerOfGame function.
6. Data structure that allows saving the sequence of moves in a game (implemented). In fileSettings.c script, the format changable with matrixToLog and logToMatrix functions.
7. The program allow writing and reading in mode of binary and text (implemented). In fileSettings.c script, writing and reading is possible with writeBinaryFile, readBinaryFile, writeTextFile, readTextFile functions.
8. Option to play against the computer (implemented). In game.c script, player can play against to computer with computerMove function.
9. The board with variable dimension (implemented).   
10. The game in 3 dimensions (implemented). In fileSettings.c script, the game playable in 3 dimensions with controlOfDimensionalGame, checkDimension functions.
