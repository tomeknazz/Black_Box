//This program will be an implementation of the Black Box game
/*
*
Implementation of a console application allowing the Black Box game to be played. The application should
allow to play several stages of varying difficulty. The stages differ in the size of the board and
number of hidden atoms. The boards can be 5×5, 8×8, 10×10. The number of atoms can be
from 3 (for the smallest board) to 8 for the largest board. The atoms hidden on the board should be
arranged in a random manner. For each stage, the screen should display the number of
of hidden atoms. Shots towards the atoms can be taken from the position of each wall. Each shot
should be numbered. The same number should mark the exit of the beam. In addition to
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
- start a new game from any stage,
In addition to this, the player should be able to undo and redo moves previously undone.
NOTE: the atoms are invisible to the player during the game!

*/

#include <iostream>
#include <iomanip>
#include <conio.h>
#include<clocale>


using namespace std;

int main();

void black_box_ascii_art() {
	cout <<
		"______  _               _     ______                _____                         \n"
		"| ___ \\| |             | |    | ___ \\              |  __ \\                        \n"
		"| |_/ /| |  __ _   ___ | | __ | |_/ /  ___  __  __ | |  \\/  __ _  _ __ ___    ___ \n"
		"| ___ \\| | / _` | / __|| |/ / | ___ \\ / _ \\ \\ \\/ / | | __  / _` || '_ ` _ \\  / _ \\\n"
		"| |_/ /| || (_| || (__ |   <  | |_/ /| (_) | >  <  | |_\\ \\| (_| || | | | | ||  __/\n"
		"\\____/ |_| \\__,_| \\___||_|\\_\\ \\____/  \\___/ /_/\\_\\  \\____/ \\__,_||_| |_| |_| \\___|\n"
		"        Tomasz Nazar                 s197613                    ACiR_3 \n"
		"                                                                                   \n";
}

void main_menu()
{
	black_box_ascii_art();
	cout << "1. Start new game" << endl;
	cout << "2. Exit" << endl;
}

void game_size()
{
	cout << "1. 5x5" << endl;
	cout << "2. 8x8" << endl;
	cout << "3. 10x10" << endl;
	cout << "4. Back" << endl;
}

void game_5_atom_number()
{
	cout << "1. 3 atoms" << endl;
	cout << "2. 4 atoms" << endl;
	cout << "3. 5 atoms" << endl;
	cout << "4. Back" << endl;
}

void game_8_atom_number()
{
	cout << "1. 3 atoms" << endl;
	cout << "2. 4 atoms" << endl;
	cout << "3. 5 atoms" << endl;
	cout << "4. 6 atoms" << endl;
	cout << "5. 7 atoms" << endl;
	cout << "6. 8 atoms" << endl;
	cout << "7. Back" << endl;
}

void game_10_atom_number()
{
	cout << "1. 3 atoms" << endl;
	cout << "2. 4 atoms" << endl;
	cout << "3. 5 atoms" << endl;
	cout << "4. 6 atoms" << endl;
	cout << "5. 7 atoms" << endl;
	cout << "6. 8 atoms" << endl;
	cout << "7. Back" << endl;
}

void horizontal_separator(const int size) {
	for (int i = 0; i < 4 * (size + 1); ++i) {
		cout << "-";
	}
	cout << "\n";
}

void header(const int size) {
	cout << setw(2) << " " << " |";

	for (int col = 1; col <= size; ++col) {
		cout << setw(2) << " " << col << " ";
	}
	cout << "\n";
}

void place_x_in_random_position(char*& array, const int size, const int atom_number) {
	// Initialize the array with dots
	for (int i = 0; i < size * size; ++i) {
		array[i] = '.';
	}

	// Place 'X' in random positions
	for (int i = 0; i < atom_number; ++i) {
		int random_position = rand() % (size * size);
		while (array[random_position] == 'X')
		{
			random_position = rand() % (size * size);
		}
		array[random_position] = 'X';

	}
}

void draw_board(const int size, const int atom_number) {
	//Dynamic array
	auto board = new char[size * size];
	place_x_in_random_position(board, size, atom_number);

	header(size);

	horizontal_separator(size);

	//Table body
	for (int row = 0; row < size; ++row) {
		cout << setw(2) << row + 1 << " |"; // Adjust row numbering

		for (int col = 0; col < size; ++col) {
			cout << setw(4) << board[row * size + col];
		}
		cout << "\n";
		horizontal_separator(size);
	}
	delete[] board;
}



int main()
{
	srand(static_cast<unsigned int>(time(0)));
	int choice;
	main_menu();
	cin >> choice;
	if (choice == 2)
		return 0;
	else if (choice == 1) {
		system("cls");
		game_size();
		cin >> choice;
		if (choice == 1) {
			system("cls");
			game_5_atom_number();
			cin >> choice;
			if (choice == 1) {
				cout << "5x5 3 atoms" << endl;
				draw_board(5, choice + 2);
			}
			else if (choice == 2) {
				cout << "5x5 4 atoms" << endl;
				draw_board(5, choice + 2);
			}
			else if (choice == 3) {
				cout << "5x5 5 atoms" << endl;
				draw_board(5, choice + 2);
			}
			else if (choice == 4) {
				system("cls");
				main();
			}
		}
		else if (choice == 2) {
			system("cls");
			game_8_atom_number();
			cin >> choice;
			if (choice == 1) {
				cout << "8x8 3 atoms" << endl;
				draw_board(8, choice + 2);
			}
			else if (choice == 2) {
				cout << "8x8 4 atoms" << endl;
				draw_board(8, choice + 2);
			}
			else if (choice == 3) {
				cout << "8x8 5 atoms" << endl;
				draw_board(8, choice + 2);
			}
			else if (choice == 4) {
				cout << "8x8 6 atoms" << endl;
				draw_board(8, choice + 2);
			}
			else if (choice == 5) {
				cout << "8x8 7 atoms" << endl;
				draw_board(8, choice + 2);
			}
			else if (choice == 6) {
				cout << "8x8 8 atoms" << endl;
				draw_board(8, choice + 2);
			}
			else if (choice == 7) {
				system("cls");
				main();
			}
		}
		else if (choice == 3) {
			system("cls");
			game_10_atom_number();
			cin >> choice;
			if (choice == 1) {
				cout << "10x10 3 atoms" << endl;
				draw_board(10, choice + 2);
			}
			else if (choice == 2) {
				cout << "10x10 4 atoms" << endl;
				draw_board(10, choice + 2);
			}
			else if (choice == 3) {
				cout << "10x10 5 atoms" << endl;
				draw_board(10, choice + 2);
			}
			else if (choice == 4) {
				cout << "10x10 6 atoms" << endl;
				draw_board(10, choice + 2);
			}
			else if (choice == 5) {
				cout << "10x10 7 atoms" << endl;
				draw_board(10, choice + 2);
			}
			else if (choice == 6) {
				cout << "10x10 8 atoms" << endl;
				draw_board(10, choice + 2);
			}
			else if (choice == 7) {
				system("cls");
				main();
			}
		}
		else if (choice == 4) {
			system("cls");
			main();
		}
	}

}

