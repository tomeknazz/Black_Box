# Black_Box

Black Box game implementation in C++

Implementation of a console application allowing the Black Box game to be played. The application will allow to play several stages of varying difficulty. The stages differ in the size of the board and number of hidden atoms. The boards can be 5×5, 8×8, 10×10. 
The number of atoms can be from 3 (for the smallest board) to 8 for the largest board. The atoms hidden on the board are arranged in a random manner. For each stage, the screen displays the number of hidden atoms. Shots towards the atoms can be taken from the position of each wall. 
Each shot is numbered. The same number should mark the exit of the beam. In addition to
the atom hit (H) and the ray reflection (R) should be marked. The supposed position of
of the atoms should be marked with a small letter o. It should be possible to change/remove the supposed position of the
of atoms. The game should allow to mark the presumed position of the atoms, equal to the number of hidden
atoms for a given stage. Each stage is evaluated in terms of the number of correctly marked
atoms. If the player decides that further gameplay no longer makes sense, he can restart the current stage.

If the player selects the supposed position of all the atoms, he should use the k (end) key,
then the program should display the position of the hidden atoms and the number. In the case of a correct
mark the position of the atoms, the letter o is replaced by O, in the case of a wrong mark, o
is replaced by X, and the letter O indicates the correct position of the atom on the board. In addition
screen should display the number of correctly marked atoms. From any stage of the game
it should be possible to go to the start menu, where you can:
- leave the game,
- start a new game from any stage

In addition to this, the player should be able to undo and redo moves previously undone.

NOTE: the atoms are invisible to the player during the game!
