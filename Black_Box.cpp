#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

struct game_state
{
	string* game_board;
	string* presumed_positions;
	int cursor_row;
	int cursor_column;
};

void menu(string* game_board, string* presumed_positions);

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
	cout << "w, s, a, d - cursor movement" << endl;
	cout << "q - exit to the main menu" << endl;
	cout << "u - undo" << endl;
	cout << "r - redo" << endl;
	cout << "space - shoot" << endl;
	cout << "o - mark the presumed position of the atom" << endl;
	cout << "k - ends the game and allows you to display the solution" << endl;
	cout << "p - display the solution (no possibility of continuing after pressed)" << endl;
	cout << "H - help/peek solution" << endl;
}

void clear_screen()
{
#if _WIN32
	system("cls");
#else
	system("clear");
#endif
}

void end_of_game(const string* game_board, const string* presumed_positions)
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

void place_x_in_random_position(string*& array, const int size, const int atom_number)
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
		if (array[randomRow * (size + 2) + randomCol] == ".")
		{
			// Place 'X' at the random position
			array[randomRow * (size + 2) + randomCol] = "X";
			--remaining_x;
		}
	}
}

void create_random_game_array(string*& array, const int size, const int atom_number)
{
	// Create an array of size (size + 2) * (size + 2)
	array = new string[(size + 2) * (size + 2)];

	// Create board
	for (int i = 0; i < size + 2; ++i)
	{
		for (int j = 0; j < size + 2; ++j)
		{
			if (i == 0 || i == size + 1 || j == 0 || j == size + 1)
				array[i * (size + 2) + j] = " ";
			else
				array[i * (size + 2) + j] = ".";
		}
	}

	place_x_in_random_position(array, size, atom_number);
}

void draw_board_hidden_atoms(string*& board, const int size, const int cursor_row, const int cursor_column, string*& presumed_positions)
{

	clear_screen();
	//Print table with cursor
	for (int i = 0; i < size + 2; i++)
	{
		for (int j = 0; j < size + 2; j++)
		{
			if (i == cursor_row && j == cursor_column)
				cout << setw(4) << '#';
			else if (presumed_positions[i * (size + 2) + j] == "o")
				cout << setw(4) << 'o';
			else if (board[i * (size + 2) + j] == "X")
				cout << setw(4) << '.';
			else if (board[i * (size + 2) + j] == "H")
				cout << setw(4) << presumed_positions[i * (size + 2) + j];
			else if (board[i * (size + 2) + j] == "R")
				cout << setw(4) << presumed_positions[i * (size + 2) + j];
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

void reflect_left();
void reflect_right();
void reflect_up();
void reflect_down();

void check_for_edge_scenarios(string*& array, const int game_size, const int cursor_row, const int cursor_column, string*& positions, const string& beam_nr, const string& shot_from)
{
	string h = "H";
	string r = "R";
	if (shot_from == "top")
	{
		//check for hit near the edge
		if (array[game_size + 2 + cursor_column] == "X")
		{
			// Mark the hit atom
			positions[cursor_column] = h.append(beam_nr);
			array[cursor_column] = 'H';
		}
		//check for atom near the edge
		if (array[game_size + 2 + cursor_column - 1] == "X" or array[game_size + 2 + cursor_column + 1] == "X")
		{
			positions[cursor_column] = r.append(beam_nr);
			array[cursor_column] = 'R';
		}
	}
	else if (shot_from == "bottom")
	{
		//check for hit near the edge
		if (array[cursor_column + (game_size + 2) * game_size] == "X")
		{
			// Mark the hit atom
			positions[cursor_column + (game_size + 2) * (game_size + 1)] = h.append(beam_nr);
			array[cursor_column + (game_size + 2) * (game_size + 1)] = 'H';
		}
		//check for atom near the edge
		else if (array[cursor_column + (game_size + 2) * game_size - 1] == "X" or array[cursor_column + (game_size + 2) * game_size + 1] == "X")
		{
			positions[cursor_column + (game_size + 2) * (game_size + 1)] = r.append(beam_nr);
			array[cursor_column + (game_size + 2) * (game_size + 1)] = 'R';
		}
	}
	else if (shot_from == "left")
	{
		//check for hit near the edge
		if (array[cursor_row * (game_size + 2) + 1] == "X")
		{
			// Mark the hit atom
			positions[cursor_row * (game_size + 2)] = h.append(beam_nr);
			array[cursor_row * (game_size + 2)] = 'H';
		}
		//check for atom near the edge
		else if (array[(cursor_row - 1) * (game_size + 2) + 1] == "X" or array[(cursor_row + 1) * (game_size + 2) + 1] == "X")
		{
			positions[cursor_row * (game_size + 2)] = r.append(beam_nr);
			array[cursor_row * (game_size + 2)] = 'R';
		}
	}
	else if (shot_from == "right")
	{
		if (array[(game_size + 1) * cursor_row - 1] == "X")
		{
			positions[(game_size + 1) * cursor_row] = h.append(beam_nr);
			array[(game_size + 1) * cursor_row] = 'H';
		}
		if (array[(game_size + 1) * (cursor_row - 1) - 1] == "X" or array[(game_size + 1) * (cursor_row + 1) - 1] == "X")
		{
			positions[(game_size + 1) * cursor_row] = r.append(beam_nr);
			array[(game_size + 1) * cursor_row] = 'R';
		}
	}

}

void check_for_reflection(string*& array, const int game_size, const int cursor_row, const int cursor_column, string*& positions, const string& beam, const string& shot_from)
{
	string h = "H";
	string r = "R";
	if (shot_from == "top")
	{
		//check for hit near the edge
		if (array[game_size + 2 + cursor_column] == "X")
		{
			// Mark the hit atom
			positions[cursor_column] = h.append(beam);
			array[cursor_column] = 'H';
		}
		//check for atom near the edge
		if (array[game_size + 2 + cursor_column - 1] == "X" or array[game_size + 2 + cursor_column + 1] == "X")
		{
			positions[cursor_column] = r.append(beam);
			array[cursor_column] = 'R';
		}
	}
	else if (shot_from == "bottom")
	{
		//check for hit near the edge
		if (array[game_size + 2 + cursor_column] == "X")
		{
			// Mark the hit atom
			positions[cursor_column] = h.append(beam);
			array[cursor_column] = 'H';
		}
		//check for atom near the edge
		if (array[game_size + 2 + cursor_column - 1] == "X" or array[game_size + 2 + cursor_column + 1] == "X")
		{
			positions[cursor_column] = r.append(beam);
			array[cursor_column] = 'R';
		}
	}
	else if (shot_from == "left")
	{
		//check for hit near the edge
		if (array[game_size + 2 + cursor_column] == "X")
		{
			// Mark the hit atom
			positions[cursor_column] = h.append(beam);
			array[cursor_column] = 'H';
		}
		//check for atom near the edge
		if (array[game_size + 2 + cursor_column - 1] == "X" or array[game_size + 2 + cursor_column + 1] == "X")
		{
			positions[cursor_column] = r.append(beam);
			array[cursor_column] = 'R';
		}
	}
	else if (shot_from == "right")
	{
		//check for hit
	}
}

void shoot_beam(string*& array, const int size, const int cursor_row, const int cursor_column, string*& positions)
{
	string h = "H";
	string r = "R";
	static int beam_number = 1;
	stringstream ss;
	ss << beam_number;
	const string beam = ss.str();
	// Check if the cursor is on the top side
	if (cursor_row == 0 and (cursor_column != 0 and cursor_column != size + 1))
	{
		bool hit = false;

		// Shoot beam from the top side
		for (int i = 2; i <= size; ++i)
		{
			check_for_edge_scenarios(array, size, cursor_row, cursor_column, positions, beam, "top");
			if (array[cursor_column] == "H" or array[cursor_column] == "R")
			{
				hit = true;
				break;
			}
			//check_for_reflection(array, size, cursor_row, cursor_column, positions, beam, "top");
			// Check if the beam hits an atom
			if (array[i * (size + 2) + cursor_column] == "X")
			{
				// Mark the hit atom
				positions[cursor_column] = h.append(beam);
				array[cursor_column] = 'H';
				hit = true;
				break;

			}
			// Check if the beam reflects from atom
		}
		beam_number++;
		if (!hit)
		{
			array[cursor_column] = beam;
			array[cursor_column + (size + 2) * (size + 1)] = beam;
		}
	}
	// Check if the cursor is on the bottom side
	else if (cursor_row == size + 1 and (cursor_column != 0 and cursor_column != size + 1))
	{
		bool hit = false;

		// Shoot beam from the bot side
		for (int i = 2; i <= size; ++i)
		{
			check_for_edge_scenarios(array, size, cursor_row, cursor_column, positions, beam, "bottom");
			if (array[cursor_column + (size + 2) * (size + 1)] == "H" or array[cursor_column + (size + 2) * (size + 1)] == "R")
			{
				hit = true;
				break;
			}
			//check_for_reflection(array, size, cursor_row, cursor_column, positions, beam, "bottom");
			// Check if the beam hits an atom
			if (array[i * (size + 2) + cursor_column] == "X")
			{
				// Mark the hit atom
				positions[cursor_column] = h.append(beam);
				array[cursor_column + (size + 2) * (size + 1)] = 'H';
				hit = true;
				break;
			}
			// Check if the beam reflects from atom
		}
		beam_number++;
		if (!hit)
		{
			array[cursor_column] = beam;
			array[cursor_column + (size + 2) * (size + 1)] = beam;
		}
	}
	// Check if the cursor is on the left side
	else if (cursor_column == 0 and (cursor_row != 0 and cursor_row != size + 1))
	{
		bool hit = false;

		// Shoot beam from the left
		for (int i = 2; i <= size; ++i)
		{
			check_for_edge_scenarios(array, size, cursor_row, cursor_column, positions, beam, "left");
			if (array[cursor_row * (size + 2)] == "H" or array[cursor_row * (size + 2)] == "R")
			{
				hit = true;
				break;
			}
			//check_for_reflection(array, size, cursor_row, cursor_column, positions, beam, "left");
			// Check if the beam hits an atom
			if (array[i * (size + 2) + cursor_column] == "X")
			{
				// Mark the hit atom
				positions[cursor_row * (size + 2)] = h.append(beam);
				array[cursor_row * (size + 2)] = 'H';
				hit = true;
				break;
			}
			// Check if the beam reflects from atom
		}
		beam_number++;
		if (!hit)
		{
			array[cursor_column] = beam;
			array[cursor_column + (size + 2) * (size + 1)] = beam;
		}
	}
	else if (cursor_column == size + 1 and (cursor_row != 0 and cursor_row != size + 1))
	{
		bool hit = false;

		// Shoot beam from the right side
		for (int i = 2; i <= size; ++i)
		{
			check_for_edge_scenarios(array, size, cursor_row, cursor_column, positions, beam, "right");
			if (array[cursor_column + (size + 2) * (size + 1)] == "H" or array[cursor_column + (size + 2) * (size + 1)] == "R")
			{
				hit = true;
				break;
			}
			//check_for_reflection(array, size, cursor_row, cursor_column, positions, beam, "right");
			// Check if the beam hits an atom
			if (array[cursor_row + i * cursor_column] == "X")
			{
				// Mark the hit atom
				positions[cursor_column] = h.append(beam);
				array[cursor_column + (size + 2) * (size + 1)] = 'H';
				hit = true;
				break;
			}
			// Check if the beam reflects from atom
		}
		beam_number++;
		if (!hit)
		{
			array[cursor_column] = beam;
			array[cursor_column + (size + 2) * (size + 1)] = beam;
		}
	}
}

void draw_board_shown_atoms(const string* game_board, const int size)
{
	clear_screen();
	//Print table with cursor
	for (int i = 0; i < size + 2; i++)
	{
		for (int j = 0; j < size + 2; j++)
		{
			if (game_board[i * (size + 2) + j] == "H")
				cout << setw(4) << " ";
			else if (game_board[i * (size + 2) + j] == "R")
				cout << setw(4) << " ";
			else
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

string convert_pointer_to_string(const string* pointer, const int game_size)
{
	string game_board;
	for (int i = 0; i < (game_size + 2) * (game_size + 2); i++)
		game_board += pointer[i];
	return game_board;
}

void write_history(game_state*& history, int& history_size, string* game_board, int cursor_row, int cursor_column, string* presumed_positions)
{
	history[history_size].game_board = game_board;
	history[history_size].cursor_row = cursor_row;
	history[history_size].cursor_column = cursor_column;
	history[history_size].presumed_positions = presumed_positions;
	history_size++;
}

void undo(game_state*& history, int& history_size, game_state*& redo_history, int& redo_history_size, int& cursor_row, int& cursor_column)
{
	if (history_size > 1)
	{
		redo_history[redo_history_size] = history[history_size - 1];
		redo_history_size++;
		redo_history[redo_history_size - 1].game_board = history[history_size - 1].game_board;
		redo_history[redo_history_size - 1].presumed_positions = history[history_size - 1].presumed_positions;
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
		history[history_size - 1].game_board = redo_history[redo_history_size - 1].game_board;
		history[history_size - 1].presumed_positions = redo_history[redo_history_size - 1].presumed_positions;
		cursor_row = history[history_size - 1].cursor_row;
		cursor_column = history[history_size - 1].cursor_column;
		redo_history_size--;

	}
}

void initialize_game(string* game_board, int cursor_row, int cursor_column, const int game_size, string* presumed_positions)
{
	// Initialize the first game state
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
	bool invalid_move = false;
	draw_board_hidden_atoms(game_board, game_size, cursor_row, cursor_column, presumed_positions);
	while (true)
	{
		invalid_move = false;
		char key[2];
		cin.getline(key, 2);
		if (key[0] == '\0')
			cin.getline(key, 2);

		if (key[0] == 'w' || key[0] == 'W')//move up
		{
			if (cursor_row > 0)
			{
				cursor_row--;
				write_history(history, history_size, game_board, cursor_row, cursor_column, presumed_positions);
			}
			else
			{
				invalid_move = true;
				continue;
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
			else
			{
				invalid_move = true;
				continue;
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
			else
			{
				invalid_move = true;
				continue;
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
			else
			{
				invalid_move = true;
				continue;
			}
			draw_board_hidden_atoms(game_board, game_size, cursor_row, cursor_column, presumed_positions);
		}
		else if (key[0] == ' ')//shoot
		{
			if (cursor_column == 0 or cursor_column == game_size + 1 or cursor_row == 0 or cursor_row == game_size + 1)
			{
				shoot_beam(game_board, game_size, cursor_row, cursor_column, presumed_positions);
				draw_board_hidden_atoms(game_board, game_size, cursor_row, cursor_column, presumed_positions);
				write_history(history, history_size, game_board, cursor_row, cursor_column, presumed_positions);
			}
			else
			{
				invalid_move = true;
				continue;
			}
		}
		else if (key[0] == 'q' || key[0] == 'Q')//exit to main menu
		{
			menu(game_board, presumed_positions);
		}
		else if (key[0] == 'u' || key[0] == 'U')//undo
		{
			undo(history, history_size, redo_history, redo_history_size, cursor_row, cursor_column);
			draw_board_hidden_atoms(game_board, game_size, cursor_row, cursor_column, presumed_positions);
			cursor_row = history[history_size - 1].cursor_row;
			cursor_column = history[history_size - 1].cursor_column;
		}
		else if (key[0] == 'r' || key[0] == 'R')//redo
		{
			redo(history, history_size, redo_history, redo_history_size, cursor_row, cursor_column);
			draw_board_hidden_atoms(game_board, game_size, cursor_row, cursor_column, presumed_positions);
			cursor_row = history[history_size - 1].cursor_row;
			cursor_column = history[history_size - 1].cursor_column;
		}
		else if (key[0] == 'o' || key[0] == 'O')//mark the presumed position of the atom
		{
			if (cursor_column == 0 or cursor_column == game_size + 1 or cursor_row == 0 or cursor_row == game_size + 1)
			{
				invalid_move = true;
				continue;
			}
			if (presumed_positions[(cursor_row) * (game_size + 2) + (cursor_column)] == "o")
				presumed_positions[(cursor_row) * (game_size + 2) + (cursor_column)] = ".";
			else
				presumed_positions[(cursor_row) * (game_size + 2) + (cursor_column)] = "o";
			write_history(history, history_size, game_board, cursor_row, cursor_column, presumed_positions);
			draw_board_hidden_atoms(game_board, game_size, cursor_row, cursor_column, presumed_positions);
		}
		else if (key[0] == 'k' || key[0] == 'K')//ends the game and allows you to display the solution
		{
			int points = 0;
			for (int i = 0; i < (game_size + 2) * (game_size + 2); i++)
			{
				if (game_board[i] == "X" && presumed_positions[i] == "o")
				{
					game_board[i] = "O";
					points++;
				}
				else if (game_board[i] == "X" && presumed_positions[i] != "o")
				{
					game_board[i] = "X";
				}
			}
			clear_screen();
			draw_board_shown_atoms(game_board, game_size);
			cout << "You earned " << points;
			if (points == 1)
				cout << " point!" << endl;
			else
				cout << " points!" << endl;

			cout << "Congratulations!!!" << endl;
			cout << "Press enter to exit the game..." << endl;
			cin.get();
			end_of_game(game_board, presumed_positions);

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
		if (invalid_move)
		{
			invalid_move = false;
			continue;
		}
	}
}

void game_choice(string* game_board, int choice, string* presumed_positions)
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
				presumed_positions = new string[49];
				initialize_game(game_board, 0, 0, 5, presumed_positions);
			}
			else if (choice == 2)
			{
				clear_screen();
				cout << "Started game: 5x5 4 atoms" << endl;
				create_random_game_array(game_board, 5, choice + 2);
				presumed_positions = new string[49];
				initialize_game(game_board, 0, 0, 5, presumed_positions);
			}
			else if (choice == 3)
			{
				clear_screen();
				cout << "Started game: 5x5 5 atoms" << endl;
				create_random_game_array(game_board, 5, choice + 2);
				presumed_positions = new string[49];
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
				presumed_positions = new string[100];
				initialize_game(game_board, 0, 0, 8, presumed_positions);
			}
			else if (choice == 2)
			{
				clear_screen();
				cout << "Started game: 8x8 4 atoms" << endl;
				create_random_game_array(game_board, 8, choice + 2);
				presumed_positions = new string[100];
				initialize_game(game_board, 0, 0, 8, presumed_positions);
			}
			else if (choice == 3)
			{
				clear_screen();
				cout << "Started game: 8x8 5 atoms" << endl;
				create_random_game_array(game_board, 8, choice + 2);
				presumed_positions = new string[100];
				initialize_game(game_board, 0, 0, 8, presumed_positions);
			}
			else if (choice == 4)
			{
				clear_screen();
				cout << "Started game: 8x8 6 atoms" << endl;
				create_random_game_array(game_board, 8, choice + 2);
				presumed_positions = new string[100];
				initialize_game(game_board, 0, 0, 8, presumed_positions);
			}
			else if (choice == 5)
			{
				clear_screen();
				cout << "Started game: 8x8 7 atoms" << endl;
				create_random_game_array(game_board, 8, choice + 2);
				presumed_positions = new string[100];
				initialize_game(game_board, 0, 0, 8, presumed_positions);
			}
			else if (choice == 6)
			{
				clear_screen();
				cout << "Started game: 8x8 8 atoms" << endl;
				create_random_game_array(game_board, 8, choice + 2);
				presumed_positions = new string[100];
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
				presumed_positions = new string[144];
				initialize_game(game_board, 0, 0, 10, presumed_positions);
			}
			else if (choice == 2)
			{
				clear_screen();
				cout << "Started game: 10x10 4 atoms" << endl;
				create_random_game_array(game_board, 10, choice + 2);
				presumed_positions = new string[144];
				initialize_game(game_board, 0, 0, 10, presumed_positions);
			}
			else if (choice == 3)
			{
				clear_screen();
				cout << "Started game: 10x10 5 atoms" << endl;
				create_random_game_array(game_board, 10, choice + 2);
				presumed_positions = new string[144];
				initialize_game(game_board, 0, 0, 10, presumed_positions);
			}
			else if (choice == 4)
			{
				clear_screen();
				cout << "Started game: 10x10 6 atoms" << endl;
				create_random_game_array(game_board, 10, choice + 2);
				presumed_positions = new string[144];
				initialize_game(game_board, 0, 0, 10, presumed_positions);
			}
			else if (choice == 5)
			{
				clear_screen();
				cout << "Started game: 10x10 7 atoms" << endl;
				create_random_game_array(game_board, 10, choice + 2);
				presumed_positions = new string[144];
				initialize_game(game_board, 0, 0, 10, presumed_positions);
			}
			else if (choice == 6)
			{
				clear_screen();
				cout << "Started game: 10x10 8 atoms" << endl;
				create_random_game_array(game_board, 10, choice + 2);
				presumed_positions = new string[144];
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

void menu(string* game_board, string* presumed_positions)
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
	string* game_board = nullptr;
	string* presumed_positions = nullptr;
	srand(time(nullptr));

	menu(game_board, presumed_positions);
	delete[] game_board;
	return 0;
}
