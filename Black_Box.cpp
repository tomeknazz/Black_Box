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

If the player selects the supposed position of all the atoms, he should use the k (end) key[0],
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
#include <cstring>

using namespace std;

struct game_state
{
	char* game_board;
	char* presumed_positions;
	int cursor_row;
	int cursor_column;
};

void menu(char* game_board, char* presumed_positions);

void black_box_ascii_art()
{
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

void clear_screen()
{
#if _WIN32
	system("cls");
#else
	system("clear");
#endif
}

void end_of_game(const char* game_board, const char* presumed_positions)
{
	clear_screen();
	cout << " _____ _   _______   ___________   _____   ___  ___  ___ _____ " << endl
		<< "|  ___| \\ | |  _  \\ |  _  |  ___| |  __ \\ / _ \\ |  \\/  ||  ___|" << endl
		<< "| |__ |  \\| | | | | | | | | |_    | |  \\// /_\\ \\| .  . || |__  " << endl
		<< "|  __|| . ` | | | | | | | |  _|   | | __ |  _  || |\\/| ||  __|" << endl
		<< "| |___| |\\  | |/ /  \\ \\_/ / |     | |_\\ \\| | | || |  | || |___" << endl
		<< "\\____/\\_| \\_/___/    \\___/\\_|      \\____/\\_| |_/\\_|  |_/\\____/ " << endl;
	delete[] game_board;
	delete[] presumed_positions;
	exit(0);
}

void place_x_in_random_position(char*& array, const int size, const int atom_number)
{
	int remaining_x = atom_number;

	// Place 'X' in random positions
	while (remaining_x > 0)
	{
		// Generate a random position
		int randomPosition = rand() % (size * size) + 1;
		int randomRow = (randomPosition - 1) / size + 1;
		int randomCol = (randomPosition - 1) % size + 1;

		// Check if the position is a dot and not already occupied by 'X'
		if (array[randomRow * (size + 2) + randomCol] == '.')
		{
			// Place 'X' at the random position
			array[randomRow * (size + 2) + randomCol] = 'X';
			--remaining_x;
		}
	}
}

void create_random_game_array(char*& array, const int size, const int atom_number)
{
	// Create an array of size (size + 2) * (size + 2)
	array = new char[(size + 2) * (size + 2)];

	// Create board
	for (int i = 0; i < size + 2; ++i)
	{
		for (int j = 0; j < size + 2; ++j)
		{
			if (i == 0 || i == size + 1 || j == 0 || j == size + 1)
				array[i * (size + 2) + j] = ' ';
			else
				array[i * (size + 2) + j] = '.';
		}
	}

	place_x_in_random_position(array, size, atom_number);
}

void draw_board_hidden_atoms(const char* board, const int size, const int cursor_row, const int cursor_column, char* presumed_positions)
{
	clear_screen();
	//Print table with cursor
	for (int i = 0; i < size + 2; i++)
	{
		for (int j = 0; j < size + 2; j++)
		{
			if (i == cursor_row && j == cursor_column)
				cout << setw(4) << '#';
			else if (presumed_positions[i * (size + 2) + j] == 'o')
				cout << setw(4) << 'o';
			else if (board[i * (size + 2) + j] == 'X')
				cout << setw(4) << '.';
			else
				cout << setw(4) << board[i * (size + 2) + j];

		}
		if (i != size + 1)
		{
			cout << "\n";
			cout << setw(4) << '-';
			for (int i = 4; i < 4 * (size + 2); ++i)
			{
				cout << "-";
			}
		}
		cout << endl;
	}
}

/*
 void move_left(char*& array, const int size, int& cursor_row, int& cursor_column)
{
	if (cursor_column > 0)
		--cursor_column;
}

void move_right(char*& array, const int size, int& cursor_row, int& cursor_column)
{
	if (cursor_column < size + 1)
		++cursor_column;
}

void move_up(char*& array, const int size, int& cursor_row, int& cursor_column)
{
	if (cursor_row > 0)
		--cursor_row;
}

void move_down(char*& array, const int size, int& cursor_row, int& cursor_column)
{
	if (cursor_row < size + 1)
		++cursor_row;
}

void check_if_atom();

void check_if_reflection();

 */

void shoot_beam_from_side(char*& array, const int size, const int cursor_row, const int cursor_column)
{
	// Check if the cursor is on the top side
	if (cursor_row == 0)
	{
		// Shoot beam from the top side
		for (int i = 1; i <= size; ++i)
		{
			// Check if the beam hits an atom
			if (array[i * (size + 2) + cursor_column] == 'X')
			{
				// Mark the hit atom
				array[cursor_column] = 'H';
				break;
			}
			// Check if the beam reflects from and atom

		}
	}
	// Check if the cursor is on the bottom side
	else if (cursor_row == size + 1)
	{
		// Shoot beam from the bottom side
		for (int i = size; i >= 1; --i)
		{
			// Check if the beam hits an atom
			if (array[i * (size + 2) + cursor_column] == 'X')
			{
				// Mark the hit atom
				array[cursor_row * (size + 2) + cursor_column] = 'H';
				break;
			}
			// Check if the beam hits a reflection

		}
	}
	// Check if the cursor is on the left side
	else if (cursor_column == 0)
	{
		// Shoot beam from the left side
		for (int i = 1; i <= size; ++i)
		{
			// Check if the beam hits an atom
			if (array[cursor_row * (size + 2) + i] == 'X')
			{
				// Mark the hit atom
				array[cursor_row * (size + 2)] = 'H';
				break;
			}
			// Check if the beam hits
		}
	}
	else
	{
		for (int i = size; i >= 1; --i)
		{
			// Check if the beam hits an atom
			if (array[cursor_row * (size + 2) + i] == 'X')
			{
				// Mark the hit atom
				array[(cursor_row + 1) * (size + 2) - 1] = 'H';
				break;
			}
			// Check if the beam hits
		}
	}

}

void draw_board_shown_atoms(const char* game_board, const int size)
{
	clear_screen();
	//Print table with cursor
	for (int i = 0; i < size + 2; i++)
	{
		for (int j = 0; j < size + 2; j++)
		{
			cout << setw(4) << game_board[i * (size + 2) + j];
		}
		if (i != size + 1)
		{
			cout << "\n";
			cout << setw(4) << '-';
			for (int i = 4; i < 4 * (size + 2); ++i)
			{
				cout << "-";
			}
		}
		cout << endl;
	}
}

void write_history(game_state*& history, int& history_size, char* game_board, int cursor_row, int cursor_column, char* presumed_positions)
{
	history[history_size].game_board = _strdup(game_board);
	history[history_size].cursor_row = cursor_row;
	history[history_size].cursor_column = cursor_column;
	history[history_size].presumed_positions = _strdup(presumed_positions);
	history_size++;
}

void undo(game_state*& history, int& history_size, game_state*& redo_history, int& redo_history_size, int& cursor_row, int& cursor_column)
{
	if (history_size > 1)
	{
		redo_history[redo_history_size] = history[history_size - 1];
		redo_history_size++;
		redo_history[redo_history_size - 1].game_board = _strdup(history[history_size - 1].game_board);
		redo_history[redo_history_size - 1].presumed_positions = _strdup(history[history_size - 1].presumed_positions);
		cursor_row = history[history_size - 1].cursor_row;
		cursor_column = history[history_size - 1].cursor_column;
		history_size--;
	}
}

void redo(game_state*& history, int& history_size, game_state*& redo_history, int& redo_history_size, int& cursor_row, int& cursor_column)
{
	if (redo_history_size > 0)
	{
		history[history_size] = redo_history[redo_history_size - 1];
		history_size++;
		history[history_size - 1].game_board = _strdup(redo_history[redo_history_size - 1].game_board);
		history[history_size - 1].presumed_positions = _strdup(redo_history[redo_history_size - 1].presumed_positions);
		cursor_row = history[history_size - 1].cursor_row;
		cursor_column = history[history_size - 1].cursor_column;
		redo_history_size--;

	}
}

void initialize_game(char* game_board, int cursor_row, int cursor_column, int game_size, char* presumed_positions)
{
	game_state initial_state;
	initial_state.game_board = game_board;
	initial_state.cursor_row = cursor_row;
	initial_state.cursor_column = cursor_column;
	initial_state.presumed_positions = presumed_positions;
	game_state* history = new game_state[100];
	game_state* redo_history = new game_state[100];
	int history_size = 1;
	int redo_history_size = 0;
	history[0] = initial_state;
	draw_board_hidden_atoms(game_board, game_size, cursor_row, cursor_column, presumed_positions);
	char key[2];
	cin.getline(key, 2);
	cout << key[0] << endl;
	while (true)
	{
		cin.getline(key, 2);


		//tutaj wstawic ifa zeby usunac reszte historii w gore jesli cos sie zmieni wzgledem redo


		if (key[0] == 'w' || key[0] == 'W')//move up
		{
			if (cursor_row > 0)
			{
				cursor_row--;
				write_history(history, history_size, game_board, cursor_row, cursor_column, presumed_positions);
			}
			draw_board_hidden_atoms(game_board, game_size, cursor_row, cursor_column, presumed_positions);
		}
		else if (key[0] == 's' || key[0] == 'S')//move down
		{
			if (cursor_row <= game_size)
			{
				cursor_row++;
				write_history(history, history_size, game_board, cursor_row, cursor_column, presumed_positions);
			}
			draw_board_hidden_atoms(game_board, game_size, cursor_row, cursor_column, presumed_positions);
		}
		else if (key[0] == 'a' || key[0] == 'A')//move left
		{
			if (cursor_column > 0)
			{
				cursor_column--;
				write_history(history, history_size, game_board, cursor_row, cursor_column, presumed_positions);
			}
			draw_board_hidden_atoms(game_board, game_size, cursor_row, cursor_column, presumed_positions);
		}
		else if (key[0] == 'd' || key[0] == 'D')//move right
		{
			if (cursor_column <= game_size)
			{
				cursor_column++;
				write_history(history, history_size, game_board, cursor_row, cursor_column, presumed_positions);
			}
			draw_board_hidden_atoms(game_board, game_size, cursor_row, cursor_column, presumed_positions);
		}
		else if (key[0] == ' ')//shoot
		{
			clear_screen();
			shoot_beam_from_side(game_board, game_size, cursor_row, cursor_column);
			draw_board_hidden_atoms(game_board, game_size, cursor_row, cursor_column, presumed_positions);
			write_history(history, history_size, game_board, cursor_row, cursor_column, presumed_positions);
		}
		else if (key[0] == 'q' || key[0] == 'Q')//exit to main menu
		{
			menu(game_board, presumed_positions);
		}
		else if (key[0] == 'u' || key[0] == 'U')//undo
		{
			undo(history, history_size, redo_history, redo_history_size, cursor_row, cursor_column);
			draw_board_hidden_atoms(history[history_size].game_board, game_size, cursor_row, cursor_column, history[history_size].presumed_positions);
		}
		else if (key[0] == 'r' || key[0] == 'R')//redo
		{
			redo(history, history_size, redo_history, redo_history_size, cursor_row, cursor_column);
			draw_board_hidden_atoms(history[history_size - 1].game_board, game_size, cursor_row, cursor_column, history[history_size - 1].presumed_positions);

		}
		else if (key[0] == 'o' || key[0] == 'O')//mark the presumed position of the atom
		{
			if (presumed_positions[(cursor_row) * (game_size + 2) + (cursor_column)] == 'o')
				presumed_positions[(cursor_row) * (game_size + 2) + (cursor_column)] = '.';
			else
				presumed_positions[(cursor_row) * (game_size + 2) + (cursor_column)] = 'o';
			write_history(history, history_size, game_board, cursor_row, cursor_column, presumed_positions);
			draw_board_hidden_atoms(game_board, game_size, cursor_row, cursor_column, presumed_positions);
		}
		else if (key[0] == 'k' || key[0] == 'K')//ends the game and allows you to display the solution
		{
			menu(game_board, presumed_positions);
		}
		else if (key[0] == 'p' || key[0] == 'P')//display the solution
		{
			clear_screen();
			draw_board_shown_atoms(game_board, game_size);
			cout << "Solution shown. Exiting to main menu. Press enter to continue..." << endl;
			cin.get();
			menu(game_board, presumed_positions);
		}
		else if (key[0] == 'h' || key[0] == 'H')//help
		{
			clear_screen();
			draw_board_shown_atoms(game_board, game_size);
			display_controls();
			cout << "Press enter to continue the game..." << endl;
			cin.get();
			clear_screen();
			draw_board_hidden_atoms(game_board, game_size, cursor_row, cursor_column, presumed_positions);
		}
		else
		{
			clear_screen();
			draw_board_hidden_atoms(game_board, game_size, cursor_row, cursor_column, presumed_positions);
		}
	}
}

void game_choice(char* game_board, int choice, char* presumed_positions)
{
	if (choice == 1)
	{
		clear_screen();
		cout << "1. 5x5" << endl;
		cout << "2. 8x8" << endl;
		cout << "3. 10x10" << endl;
		cout << "4. Back" << endl;
		cin >> choice;
		if (choice == 1)
		{

			clear_screen();
			cout << "1. 3 atoms" << endl;
			cout << "2. 4 atoms" << endl;
			cout << "3. 5 atoms" << endl;
			cout << "4. Back" << endl;
			cin >> choice;
			if (choice == 1)
			{
				clear_screen();
				cout << "Started game: 5x5 3 atoms" << endl;
				create_random_game_array(game_board, 5, choice + 2);
				presumed_positions = new char[49];
				initialize_game(game_board, 0, 0, 5, presumed_positions);
			}
			else if (choice == 2)
			{
				clear_screen();
				cout << "Started game: 5x5 4 atoms" << endl;
				create_random_game_array(game_board, 5, choice + 2);
				presumed_positions = new char[49];
				initialize_game(game_board, 0, 0, 5, presumed_positions);
			}
			else if (choice == 3)
			{
				clear_screen();
				cout << "Started game: 5x5 5 atoms" << endl;
				create_random_game_array(game_board, 5, choice + 2);
				presumed_positions = new char[49];
				initialize_game(game_board, 0, 0, 5, presumed_positions);
			}
			else if (choice == 4)
			{
				clear_screen();
				menu(game_board, presumed_positions);
			}
		}
		else if (choice == 2)
		{
			clear_screen();
			cout << "1. 3 atoms" << endl;
			cout << "2. 4 atoms" << endl;
			cout << "3. 5 atoms" << endl;
			cout << "4. 6 atoms" << endl;
			cout << "5. 7 atoms" << endl;
			cout << "6. 8 atoms" << endl;
			cout << "7. Back" << endl;
			cin >> choice;
			if (choice == 1)
			{
				clear_screen();
				cout << "Started game: 8x8 3 atoms" << endl;
				create_random_game_array(game_board, 8, choice + 2);
				presumed_positions = new char[100];
				initialize_game(game_board, 0, 0, 8, presumed_positions);
			}
			else if (choice == 2)
			{
				clear_screen();
				cout << "Started game: 8x8 4 atoms" << endl;
				create_random_game_array(game_board, 8, choice + 2);
				presumed_positions = new char[100];
				initialize_game(game_board, 0, 0, 8, presumed_positions);
			}
			else if (choice == 3)
			{
				clear_screen();
				cout << "Started game: 8x8 5 atoms" << endl;
				create_random_game_array(game_board, 8, choice + 2);
				presumed_positions = new char[100];
				initialize_game(game_board, 0, 0, 8, presumed_positions);
			}
			else if (choice == 4)
			{
				clear_screen();
				cout << "Started game: 8x8 6 atoms" << endl;
				create_random_game_array(game_board, 8, choice + 2);
				presumed_positions = new char[100];
				initialize_game(game_board, 0, 0, 8, presumed_positions);
			}
			else if (choice == 5)
			{
				clear_screen();
				cout << "Started game: 8x8 7 atoms" << endl;
				create_random_game_array(game_board, 8, choice + 2);
				presumed_positions = new char[100];
				initialize_game(game_board, 0, 0, 8, presumed_positions);
			}
			else if (choice == 6)
			{
				clear_screen();
				cout << "Started game: 8x8 8 atoms" << endl;
				create_random_game_array(game_board, 8, choice + 2);
				presumed_positions = new char[100];
				initialize_game(game_board, 0, 0, 8, presumed_positions);
			}
			else if (choice == 7)
			{
				clear_screen();
				menu(game_board, presumed_positions);
			}
		}
		else if (choice == 3)
		{
			clear_screen();
			cout << "1. 3 atoms" << endl;
			cout << "2. 4 atoms" << endl;
			cout << "3. 5 atoms" << endl;
			cout << "4. 6 atoms" << endl;
			cout << "5. 7 atoms" << endl;
			cout << "6. 8 atoms" << endl;
			cout << "7. Back" << endl;
			cin >> choice;
			if (choice == 1)
			{
				clear_screen();
				cout << "Started game: 10x10 3 atoms" << endl;
				create_random_game_array(game_board, 10, choice + 2);
				presumed_positions = new char[144];
				initialize_game(game_board, 0, 0, 10, presumed_positions);
			}
			else if (choice == 2)
			{
				clear_screen();
				cout << "Started game: 10x10 4 atoms" << endl;
				create_random_game_array(game_board, 10, choice + 2);
				presumed_positions = new char[144];
				initialize_game(game_board, 0, 0, 10, presumed_positions);
			}
			else if (choice == 3)
			{
				clear_screen();
				cout << "Started game: 10x10 5 atoms" << endl;
				create_random_game_array(game_board, 10, choice + 2);
				presumed_positions = new char[144];
				initialize_game(game_board, 0, 0, 10, presumed_positions);
			}
			else if (choice == 4)
			{
				clear_screen();
				cout << "Started game: 10x10 6 atoms" << endl;
				create_random_game_array(game_board, 10, choice + 2);
				presumed_positions = new char[144];
				initialize_game(game_board, 0, 0, 10, presumed_positions);
			}
			else if (choice == 5)
			{
				clear_screen();
				cout << "Started game: 10x10 7 atoms" << endl;
				create_random_game_array(game_board, 10, choice + 2);
				presumed_positions = new char[144];
				initialize_game(game_board, 0, 0, 10, presumed_positions);
			}
			else if (choice == 6)
			{
				clear_screen();
				cout << "Started game: 10x10 8 atoms" << endl;
				create_random_game_array(game_board, 10, choice + 2);
				presumed_positions = new char[144];
				initialize_game(game_board, 0, 0, 10, presumed_positions);
			}
			else if (choice == 7)
			{
				clear_screen();
				menu(game_board, presumed_positions);
			}
		}
		else if (choice == 4)
		{
			clear_screen();
			menu(game_board, presumed_positions);
		}
	}
	else if (choice == 2)
	{
		delete[] game_board;
		exit(0);
	}
	else
	{
		clear_screen();
		menu(game_board, presumed_positions);
	}
}

void menu(char* game_board, char* presumed_positions)
{
	clear_screen();
	black_box_ascii_art();
	cout << "1. Start new game" << endl;
	cout << "2. Exit" << endl;
	int choice;
	cin >> choice;
	if (choice == 2)
	{
		end_of_game(game_board, presumed_positions);
	}
	if (choice == 1)
	{
		game_choice(game_board, choice, presumed_positions);
	}
	else
	{
		clear_screen();
		menu(game_board, presumed_positions);
	}
}

int main()
{
	char* game_board = nullptr;
	char* presumed_positions = nullptr;
	srand(time(nullptr));
	menu(game_board, presumed_positions);
	delete[] game_board;
	return 0;
}
