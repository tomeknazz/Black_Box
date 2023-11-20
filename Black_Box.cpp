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

Program powinien wykorzystywac klawiature w nastepujacy sposob:
• w, s, a, d i (W, S, A, D) – poruszanie sie po planszy odpowiednio: w gore, w dol, w lewo i
prawo;
• q, Q – wyjscie do menu glownego;
• u, U – undo (cofnij ruch);
• r, R – redo (powtorz cofniety ruch);
• spacja oddanie strzalu (gdy kursor jest na dowolnej scianie);
• o - na planszy umozliwia zaznaczenie przypuszczalnego polozenia atomu;
• k – konczy rozgrywke i umozliwia wyswietlenie rozwiazania i liczby uzyskanych punktow
(poprawnie znalezionych atomow);
• p – umozliwia wyswietlenie rozwiazania (przerywa etap gry, brak mozliwosci kontynuowania
tego etapu gry);
• H – Help/Pomoc – pokazuje na chwile umieszczenie atomow na planszy
• pozostale pozostaja do wyboru przez programiste; Klawisze sterujace powinny byc
zatwierdzane klawiszem enter.

*/

#include <iostream>
#include <iomanip>
//#include<clocale>


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

void full_board_with_dots(const int size, int cursor_row, int cursor_column) {
	char* board = new char[(size + 2) * (size + 2)];
	int counter = 0;

	// Fill the first row
	for (int i = 0; i < size + 2; ++i) {
		board[counter++] = ' ';
	}

	// Fill the internal rows
	for (int j = 0; j < size; ++j) {
		board[counter++] = ' '; // Left space
		for (int i = 0; i < size; ++i) {
			board[counter++] = '.'; // Dots
		}
		board[counter++] = ' '; // Right space
	}

	// Fill the last row
	for (int i = 0; i < size + 2; ++i) {
		board[counter++] = ' ';
	}
	for (int i = 0; i < size + 2; i++)
	{
		for (int j = 0; j < size + 2; j++)
		{
			if (i == cursor_row && j == cursor_column)
				cout << setw(4) << '#';
			else
				cout << setw(4) << board[i * (size + 2) + j];

		}
		if (i != size + 1)
		{
			cout << "\n";
			cout << setw(4) << '-';
			for (int i = 4; i < 4 * (size + 2); ++i) {
				cout << "-";
			}

		}
		cout << endl;

	}

	delete[] board;
}

void draw_board_shown_atoms(const int size, const int atom_number)
{
	auto board_shown_atoms = new char[size * size];
	place_x_in_random_position(board_shown_atoms, size, atom_number);
	//Header
	cout << setw(4) << ' ';

	for (int col = 1; col <= size + 1; ++col) {
		cout << setw(4) << ' ';
	}
	cout << "\n";
	//Horizontal line
	cout << setw(4) << '-';
	for (int i = 4; i < 4 * (size + 2); ++i) {
		cout << "-";
	}
	cout << "\n";

	//Table body
	for (int row = 0; row < size; ++row) {
		cout << setw(4) << ' '; // Adjust row numbering

		for (int col = 0; col < size; ++col) {
			cout << setw(4) << board_shown_atoms[row * size + col];
		}
		cout << setw(4) << ' ';
		cout << "\n";
		cout << setw(4) << '-';
		for (int i = 4; i < 4 * (size + 2); ++i) {
			cout << "-";
		}
		cout << "\n";
	}
	//Header
	cout << setw(4) << ' ';

	for (int col = 1; col <= size + 1; ++col) {
		cout << setw(4) << ' ';
	}
	cout << "\n";
	delete[] board_shown_atoms;
}

void draw_board_hidden_atoms(const int size) {
	//Dynamic array
	auto board_hidden_atoms = new char[(size + 2) * (size + 2)];

}

void game_choice(int choice)
{
	if (choice == 1) {
		system("cls");
		cout << "1. 5x5" << endl;
		cout << "2. 8x8" << endl;
		cout << "3. 10x10" << endl;
		cout << "4. Back" << endl;
		cin >> choice;
		if (choice == 1) {
			system("cls");
			cout << "1. 3 atoms" << endl;
			cout << "2. 4 atoms" << endl;
			cout << "3. 5 atoms" << endl;
			cout << "4. Back" << endl;
			cin >> choice;
			if (choice == 1) {
				cout << "5x5 3 atoms" << endl;
				draw_board_shown_atoms(5, choice + 2);
			}
			else if (choice == 2) {
				cout << "5x5 4 atoms" << endl;
				draw_board_shown_atoms(5, choice + 2);
			}
			else if (choice == 3) {
				cout << "5x5 5 atoms" << endl;
				draw_board_shown_atoms(5, choice + 2);
			}
			else if (choice == 4) {
				system("cls");
				main();
			}
		}
		else if (choice == 2) {
			system("cls");
			cout << "1. 3 atoms" << endl;
			cout << "2. 4 atoms" << endl;
			cout << "3. 5 atoms" << endl;
			cout << "4. 6 atoms" << endl;
			cout << "5. 7 atoms" << endl;
			cout << "6. 8 atoms" << endl;
			cout << "7. Back" << endl;
			cin >> choice;
			if (choice == 1) {
				cout << "8x8 3 atoms" << endl;
				draw_board_shown_atoms(8, choice + 2);
			}
			else if (choice == 2) {
				cout << "8x8 4 atoms" << endl;
				draw_board_shown_atoms(8, choice + 2);
			}
			else if (choice == 3) {
				cout << "8x8 5 atoms" << endl;
				draw_board_shown_atoms(8, choice + 2);
			}
			else if (choice == 4) {
				cout << "8x8 6 atoms" << endl;
				draw_board_shown_atoms(8, choice + 2);
			}
			else if (choice == 5) {
				cout << "8x8 7 atoms" << endl;
				draw_board_shown_atoms(8, choice + 2);
			}
			else if (choice == 6) {
				cout << "8x8 8 atoms" << endl;
				draw_board_shown_atoms(8, choice + 2);
			}
			else if (choice == 7) {
				system("cls");
				main();
			}
		}
		else if (choice == 3) {
			system("cls");
			cout << "1. 3 atoms" << endl;
			cout << "2. 4 atoms" << endl;
			cout << "3. 5 atoms" << endl;
			cout << "4. 6 atoms" << endl;
			cout << "5. 7 atoms" << endl;
			cout << "6. 8 atoms" << endl;
			cout << "7. Back" << endl;
			cin >> choice;
			if (choice == 1) {
				cout << "10x10 3 atoms" << endl;
				draw_board_shown_atoms(10, choice + 2);
			}
			else if (choice == 2) {
				cout << "10x10 4 atoms" << endl;
				draw_board_shown_atoms(10, choice + 2);
			}
			else if (choice == 3) {
				cout << "10x10 5 atoms" << endl;
				draw_board_shown_atoms(10, choice + 2);
			}
			else if (choice == 4) {
				cout << "10x10 6 atoms" << endl;
				draw_board_shown_atoms(10, choice + 2);
			}
			else if (choice == 5) {
				cout << "10x10 7 atoms" << endl;
				draw_board_shown_atoms(10, choice + 2);
			}
			else if (choice == 6) {
				cout << "10x10 8 atoms" << endl;
				draw_board_shown_atoms(10, choice + 2);
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

void display_controls()
{
	cout << "Controls:" << endl;
	cout << "w, s, a, d - move the cursor up, down, left and right" << endl;
	cout << "q, Q - exit to the main menu" << endl;
	cout << "u, U - undo" << endl;
	cout << "r, R - redo" << endl;
	cout << "space - shoot" << endl;
	cout << "o - mark the presumed position of the atom" << endl;
	cout << "k - ends the game and allows you to display the solution" << endl;
	cout << "p - display the solution (no possibility of continuing after pressed)" << endl;
	cout << "H - help" << endl;
}

void movement_controls(int cursor_row, int cursor_column) {
	while (true)
	{
		char key;
		cin >> key;
		if (key == 'w' || key == 'W')
		{
			if (cursor_row > 0)
				cursor_row--;
			system("cls");
			full_board_with_dots(10, cursor_row, cursor_column);
		}
		else if (key == 's' || key == 'S')
		{
			if (cursor_row <= 10)
				cursor_row++;
			system("cls");
			full_board_with_dots(10, cursor_row, cursor_column);
		}
		else if (key == 'a' || key == 'A')
		{
			if (cursor_column > 0)
				cursor_column--;
			system("cls");
			full_board_with_dots(10, cursor_row, cursor_column);
		}
		else if (key == 'd' || key == 'D')
		{
			if (cursor_column <= 10)
				cursor_column++;
			system("cls");
			full_board_with_dots(10, cursor_row, cursor_column);
		}
		else if (key == 'q' || key == 'Q')
		{
			system("cls");
			main();
		}
		else if (key == 'u' || key == 'U')
		{
			system("cls");
			main();
		}
		else if (key == 'r' || key == 'R')
		{
			system("cls");
			main();
		}
		else if (key == ' ')
		{
			system("cls");
			main();
		}
		else if (key == 'o' || key == 'O')
		{
			system("cls");
			main();
		}
		else if (key == 'k' || key == 'K')
		{
			system("cls");
			main();
		}
		else if (key == 'p' || key == 'P')
		{
			system("cls");
			main();
		}
		else if (key == 'h' || key == 'H')
		{
			system("cls");
			display_controls();
			full_board_with_dots(10, cursor_row, cursor_column);
		}
		else
		{
			system("cls");
			full_board_with_dots(10, cursor_row, cursor_column);
		}
	}
}

int main()
{
	srand(static_cast<unsigned int>(time(NULL)));

	black_box_ascii_art();
	cout << "1. Start new game" << endl;
	cout << "2. Exit" << endl;

	movement_controls(0, 0);

	int choice;
	cin >> choice;
	if (choice == 2)
		return 0;
	else
		game_choice(choice);

	return 0;
}