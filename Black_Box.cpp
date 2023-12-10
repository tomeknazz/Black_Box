#include <iostream>
#include <iomanip>
#include <sstream>
#include <cctype>
#include <cstdlib>

using namespace std;

struct game_state
{
	int round;
	char input;
};

void menu(string* game_board, string* presumed_positions);

void clear_screen();

void black_box_ascii_art();

void end_of_game(string*& game_board, string*& presumed_positions);

void display_controls();

void place_x_in_random_position(string*& array, int size, int atom_number);

void create_random_game_array(string*& array, int size, int atom_number);

void draw_board_hidden_atoms(string*& board, int size, int cursor_row, int cursor_column, string*& presumed_positions);

void check_for_edge_scenarios(string*& array, int game_size, int cursor_row, int cursor_column, string*& positions, const string& beam_nr, const string& shot_from);

void shoot_beam(string*& array, int size, int cursor_row, int cursor_column, string*& positions);

void draw_board_shown_atoms(const string* game_board, int size);

void initialize_game(string* game_board, int cursor_row, int cursor_column, int game_size, string* presumed_positions);

void game_choice(string* game_board, int choice, string* presumed_positions);

int main()
{
	string* game_board = nullptr;
	string* presumed_positions = nullptr;
	srand(time(nullptr));
	menu(game_board, presumed_positions);
	return 0;
}

void initialize_game(string* game_board, int cursor_row, int cursor_column, const int game_size, string* presumed_positions)
{
	draw_board_hidden_atoms(game_board, game_size, cursor_row, cursor_column, presumed_positions);
	cout << endl;
	game_state moves[200]{};
	int index = 0;
	int index_max = 0;
	while (true)
	{
		cout << "\033[38;5;69m" << "   MOVES DONE: " << index << "\033[38;5;208m" << "  H - HIT " << " R - REFLECTION FROM CORNER" << "\033[0m" << endl << endl;
		char key[2];
		while (true) {
			cout << "Enter input: ";
			cin.getline(key, 2);
			key[0] = tolower(key[0]);
			if (cin.fail() || key[0] == '\0' || isdigit(key[0])) {
				cin.clear();  // clear the error flag
				cin.ignore(10, '\n');  // discard invalid input
				cout << "Invalid input. Please try again." << endl;
			}
			else
				break;  // valid input, exit the loop
		}
		if (key[0] == 'w' or key[0] == 'a' or key[0] == 's' or key[0] == 'd' or key[0] == 'o' or key[0] == ' ')
		{
			moves[index].round = index;
			moves[index].input = key[0];
			index++;
			index_max++;
		}
		if (key[0] == 'w')//move up
		{
			if (cursor_row > 0)
				cursor_row--;
			else
				continue;
			draw_board_hidden_atoms(game_board, game_size, cursor_row, cursor_column, presumed_positions);
		}
		else if (key[0] == 's')//move down
		{
			if (cursor_row <= game_size)
				cursor_row++;
			else
				continue;
			draw_board_hidden_atoms(game_board, game_size, cursor_row, cursor_column, presumed_positions);
		}
		else if (key[0] == 'a')//move left
		{
			if (cursor_column > 0)
				cursor_column--;
			else
				continue;
			draw_board_hidden_atoms(game_board, game_size, cursor_row, cursor_column, presumed_positions);
		}
		else if (key[0] == 'd')//move right
		{
			if (cursor_column <= game_size)
				cursor_column++;
			else
				continue;
			draw_board_hidden_atoms(game_board, game_size, cursor_row, cursor_column, presumed_positions);
		}
		else if (key[0] == ' ')//shoot
		{
			if (cursor_column == 0 or cursor_column == game_size + 1 or cursor_row == 0 or cursor_row == game_size + 1)
			{
				shoot_beam(game_board, game_size, cursor_row, cursor_column, presumed_positions);
				draw_board_hidden_atoms(game_board, game_size, cursor_row, cursor_column, presumed_positions);
			}
		}
		else if (key[0] == 'q')//exit to main menu
			menu(game_board, presumed_positions);
		else if (key[0] == 'u')//undo
		{
			if (index == 0)
				continue;
			index--;
			if (moves[index].input == 'w')
				cursor_row++;
			else if (moves[index].input == 's')
				cursor_row--;
			else if (moves[index].input == 'a')
				cursor_column++;
			else if (moves[index].input == 'd')
				cursor_column--;
			else if (moves[index].input == ' ')
			{
				if (cursor_row == 0 or cursor_row == game_size + 1)
				{
					if (game_board[cursor_row * (game_size + 2) + cursor_column] == "H" or game_board[cursor_row * (game_size + 2) + cursor_column] == "R")
						game_board[cursor_row * (game_size + 2) + cursor_column] = ' ';
					else if (isdigit(game_board[cursor_row * (game_size + 2) + cursor_column][0]))
					{
						game_board[cursor_row * (game_size + 2) + cursor_column] = ' ';
						game_board[(game_size + 1) * (game_size + 2) + cursor_column] = ' ';
					}
				}
				else if (cursor_column == 0 or cursor_column == game_size + 1)
				{
					if (game_board[cursor_row * (game_size + 2)] == "H" or game_board[cursor_row * (game_size + 2)] == "R")
						game_board[cursor_row * (game_size + 2)] = ' ';
					else if (isdigit(game_board[cursor_row * (game_size + 2)][0]))
					{
						game_board[cursor_row * (game_size + 2)] = ' ';
						game_board[cursor_row * (game_size + 2) + game_size + 1] = ' ';
					}
				}
			}
			else if (moves[index].input == 'o')
			{
				if (presumed_positions[(cursor_row) * (game_size + 2) + (cursor_column)] == "o")
					presumed_positions[(cursor_row) * (game_size + 2) + (cursor_column)] = ".";
				else
					presumed_positions[(cursor_row) * (game_size + 2) + (cursor_column)] = "o";
			}
			draw_board_hidden_atoms(game_board, game_size, cursor_row, cursor_column, presumed_positions);
		}
		else if (key[0] == 'r')//redo
		{
			if (index == index_max)
				continue;
			if (moves[index].input == 'w')
				cursor_row--;
			else if (moves[index].input == 's')
				cursor_row++;
			else if (moves[index].input == 'a')
				cursor_column--;
			else if (moves[index].input == 'd')
				cursor_column++;
			else if (moves[index].input == ' ')
			{
				if (cursor_column == 0 or cursor_column == game_size + 1 or cursor_row == 0 or cursor_row == game_size + 1)
				{
					shoot_beam(game_board, game_size, cursor_row, cursor_column, presumed_positions);
					draw_board_hidden_atoms(game_board, game_size, cursor_row, cursor_column, presumed_positions);
				}
			}
			else if (moves[index].input == 'o')
			{
				if (presumed_positions[(cursor_row) * (game_size + 2) + (cursor_column)] == "o")
					presumed_positions[(cursor_row) * (game_size + 2) + (cursor_column)] = ".";
				else
					presumed_positions[(cursor_row) * (game_size + 2) + (cursor_column)] = "o";
				draw_board_hidden_atoms(game_board, game_size, cursor_row, cursor_column, presumed_positions);
			}
			index++;
			draw_board_hidden_atoms(game_board, game_size, cursor_row, cursor_column, presumed_positions);
		}
		else if (key[0] == 'o')//mark the presumed position of the atom
		{
			if (cursor_column == 0 or cursor_column == game_size + 1 or cursor_row == 0 or cursor_row == game_size + 1)
				continue;
			if (presumed_positions[(cursor_row) * (game_size + 2) + (cursor_column)] == "o")
				presumed_positions[(cursor_row) * (game_size + 2) + (cursor_column)] = ".";
			else
				presumed_positions[(cursor_row) * (game_size + 2) + (cursor_column)] = "o";
			draw_board_hidden_atoms(game_board, game_size, cursor_row, cursor_column, presumed_positions);
		}
		else if (key[0] == 'k')//ends the game and allows you to display the solution
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
					game_board[i] = "X";
			}
			draw_board_shown_atoms(game_board, game_size);
			cout << "You've scored " << points;
			if (points == 1)
				cout << " point!" << endl;
			else
				cout << " points!" << endl;
			cout << "Congratulations!!!\a" << endl;
			cout << "Press enter to exit the game..." << endl;
			cin.get();
			end_of_game(game_board, presumed_positions);
		}
		else if (key[0] == 'p')//display the solution
		{
			draw_board_shown_atoms(game_board, game_size);
			cout << "Solution shown. Exiting to main menu. Press enter to continue..." << endl;
			cin.get();
			menu(game_board, presumed_positions);
		}
		else if (key[0] == 'h')//help
		{
			draw_board_shown_atoms(game_board, game_size);
			display_controls();
			cout << "Press enter to continue the game..." << endl;
			cin.get();
			draw_board_hidden_atoms(game_board, game_size, cursor_row, cursor_column, presumed_positions);
		}
		else
			draw_board_hidden_atoms(game_board, game_size, cursor_row, cursor_column, presumed_positions);
	}
}

void create_random_game_array(string*& array, const int size, const int atom_number)
{
	// Create an array
	array = new string[(size + 2) * (size + 2)];
	// Generate board
	for (int i = 0; i < size + 2; ++i)
		for (int j = 0; j < size + 2; ++j)
		{
			if (i == 0 || i == size + 1 || j == 0 || j == size + 1)
				array[i * (size + 2) + j] = " ";
			else
				array[i * (size + 2) + j] = ".";
		}
	place_x_in_random_position(array, size, atom_number);
}

void place_x_in_random_position(string*& array, const int size, const int atom_number)
{
	int remaining_x = atom_number;
	// Place X in random positions
	while (remaining_x > 0)
	{
		// Generate a random position
		int randomPosition = rand() % (size * size) + 1;
		int randomRow = (randomPosition - 1) / size + 1;
		int randomCol = (randomPosition - 1) % size + 1;

		// Check if dot and not X
		if (array[randomRow * (size + 2) + randomCol] == ".")
		{
			// Place X in random position
			array[randomRow * (size + 2) + randomCol] = "X";
			remaining_x--;
		}
	}
}

void draw_board_hidden_atoms(string*& board, const int size, const int cursor_row, const int cursor_column, string*& presumed_positions)
{
	clear_screen();
	//Print table with cursor
	cout << setw(4) << char(201);
	for (int i = 4; i < 4 * (size + 4) - 1; ++i)
		cout << char(196);
	cout << setw(1) << char(187) << endl;
	for (int i = 0; i < size + 2; i++)
	{
		cout << setw(4) << char(186);
		for (int j = 0; j < size + 2; j++)
		{
			if (i == cursor_row && j == cursor_column)
				cout << "\033[33m" << setw(4) << char(219) << "\033[0m";
			else if (presumed_positions[i * (size + 2) + j] == "o")
				cout << "\033[92m" << setw(4) << 'O' << "\033[0m";
			else if (board[i * (size + 2) + j] == "X")
				cout << setw(4) << '.';
			else if (board[i * (size + 2) + j] == "H" or board[i * (size + 2) + j] == "R")
				cout << "\033[38;5;208m" << setw(4) << presumed_positions[i * (size + 2) + j] << "\033[0m";
			else if (isdigit(board[i * (size + 2) + j][0]))
				cout << "\033[35m" << setw(4) << board[i * (size + 2) + j] << "\033[0m";
			else
				cout << setw(4) << board[i * (size + 2) + j];
		}
		cout << setw(4) << char(186);
		if (i != size + 1)
		{
			cout << "\n";
			cout << setw(4) << char(204);
			for (int i = 4; i < 4 * (size + 4) - 1; ++i)
			{
				cout << char(196);
			}
			cout << setw(1) << char(185);
		}
		cout << endl;
	}
	cout << setw(4) << char(200);
	for (int i = 4; i < 4 * (size + 4) - 1; ++i)
	{
		cout << char(196);
	}
	cout << setw(1) << char(188) << endl;
}

void draw_board_shown_atoms(const string* game_board, const int size)
{
	clear_screen();
	//Print table with cursor
	cout << setw(4) << char(201);
	for (int i = 4; i < 4 * (size + 4) - 1; ++i)
	{
		cout << char(196);
	}
	cout << setw(1) << char(187) << endl;
	for (int i = 0; i < size + 2; i++)
	{
		cout << setw(4) << char(186);
		for (int j = 0; j < size + 2; j++)
		{
			if (game_board[i * (size + 2) + j] == "H" or game_board[i * (size + 2) + j] == "R" or isdigit(game_board[i * (size + 2) + j][0]))
				cout << setw(4) << " ";
			else if (game_board[i * (size + 2) + j] == "O")
				cout << "\033[92m" << setw(4) << game_board[i * (size + 2) + j] << "\033[0m";
			else if (game_board[i * (size + 2) + j] == "X")
				cout << "\033[31m" << setw(4) << game_board[i * (size + 2) + j] << "\033[0m";
			else
				cout << setw(4) << game_board[i * (size + 2) + j];
		}
		cout << setw(4) << char(186);
		if (i != size + 1)
		{
			cout << "\n";
			cout << setw(4) << char(204);
			for (int i = 4; i < 4 * (size + 4) - 1; ++i)
			{
				cout << char(196);
			}
			cout << setw(1) << char(185);
		}
		cout << endl;
	}
	cout << setw(4) << char(200);
	for (int i = 4; i < 4 * (size + 4) - 1; ++i)
	{
		cout << char(196);
	}
	cout << setw(1) << char(188) << endl;
}

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
		if (array[(game_size + 2) * cursor_row + game_size] == "X")
		{
			positions[(game_size + 2) * cursor_row + game_size + 1] = h.append(beam_nr);
			array[(game_size + 2) * cursor_row + game_size + 1] = 'H';
		}
		if (array[(game_size + 2) * (cursor_row - 1) + game_size] == "X" or array[(game_size + 2) * (cursor_row + 1) + game_size] == "X")
		{
			positions[cursor_row * (game_size + 2) + game_size + 1] = r.append(beam_nr);
			array[cursor_row * (game_size + 2) + game_size + 1] = 'R';
		}
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
	//Top side
	if (cursor_row == 0 and (cursor_column != 0 and cursor_column != size + 1))
	{
		bool hit = false;
		for (int i = 2; i <= size; ++i)
		{
			check_for_edge_scenarios(array, size, cursor_row, cursor_column, positions, beam, "top");
			if (array[cursor_column] == "H" or array[cursor_column] == "R")
			{
				hit = true;
				break;
			}
			if (array[i * (size + 2) + cursor_column] == "X")
			{
				positions[cursor_column] = h.append(beam);
				array[cursor_column] = 'H';
				hit = true;
				break;
			}
		}
		beam_number++;
		if (!hit)
		{
			array[cursor_column] = beam;
			array[cursor_column + (size + 2) * (size + 1)] = beam;
		}
	}
	//Bottom side
	else if (cursor_row == size + 1 and (cursor_column != 0 and cursor_column != size + 1))
	{
		bool hit = false;
		for (int i = 2; i <= size; ++i)
		{
			check_for_edge_scenarios(array, size, cursor_row, cursor_column, positions, beam, "bottom");
			if (array[cursor_column + (size + 2) * (size + 1)] == "H" or array[cursor_column + (size + 2) * (size + 1)] == "R")
			{
				hit = true;
				break;
			}
			if (array[i * (size + 2) + cursor_column] == "X")
			{
				positions[cursor_column + (size + 2) * (size + 1)] = h.append(beam);
				array[cursor_column + (size + 2) * (size + 1)] = 'H';
				hit = true;
				break;
			}
		}
		beam_number++;
		if (!hit)
		{
			array[cursor_column] = beam;
			array[cursor_column + (size + 2) * (size + 1)] = beam;
		}
	}
	//Left side
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
			if (array[cursor_row * (size + 2) + i] == "X")
			{
				positions[cursor_row * (size + 2)] = h.append(beam);
				array[cursor_row * (size + 2)] = 'H';
				hit = true;
				break;
			}
		}
		beam_number++;
		if (!hit)
		{
			array[cursor_row * (size + 2)] = beam;
			array[cursor_row * (size + 2) + size + 1] = beam;
		}
	}
	//Right side
	else if (cursor_column == size + 1 and (cursor_row != 0 and cursor_row != size + 1))
	{
		bool hit = false;
		for (int i = 2; i <= size; ++i)
		{
			check_for_edge_scenarios(array, size, cursor_row, cursor_column, positions, beam, "right");
			if (array[(size + 2) * cursor_row + size + 1] == "H" or array[(size + 2) * cursor_row + size + 1] == "R")
			{
				hit = true;
				break;
			}
			if (array[cursor_row * (size + 1) + i] == "X")
			{
				positions[cursor_row * (size + 2) + size + 1] = h.append(beam);
				array[cursor_row * (size + 2) + size + 1] = 'H';
				hit = true;
				break;
			}
		}
		beam_number++;
		if (!hit)
		{
			array[cursor_row * (size + 2)] = beam;
			array[cursor_row * (size + 2) + size + 1] = beam;
		}
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

void game_choice(string* game_board, int choice, string* presumed_positions)
{
	if (choice == 1)
	{
		clear_screen();
		cout << "\033[33m" << "Choose game size: " << "\033[0m" << endl;
		cout << "1. 5x5" << endl;
		cout << "2. 8x8" << endl;
		cout << "3. 10x10" << endl;
		cout << "4. Back" << endl;
		cin >> choice;
		if (choice == 1)
		{
			clear_screen();
			cout << "\033[33m" << "Choose number of hidden atoms: " << "\033[0m" << endl;
			cout << "1. 3 atoms" << endl;
			cout << "2. 4 atoms" << endl;
			cout << "3. 5 atoms" << endl;
			cout << "4. Back" << endl;
			cin >> choice;
			if (choice == 4)
			{
				clear_screen();
				menu(game_board, presumed_positions);
			}
			else
			{
				create_random_game_array(game_board, 5, choice + 2);
				presumed_positions = new string[49];
				initialize_game(game_board, 0, 0, 5, presumed_positions);
			}
		}
		else if (choice == 2)
		{
			clear_screen();
			cout << "\033[33m" << "Choose number of hidden atoms: " << "\033[0m" << endl;
			cout << "1. 3 atoms" << endl;
			cout << "2. 4 atoms" << endl;
			cout << "3. 5 atoms" << endl;
			cout << "4. 6 atoms" << endl;
			cout << "5. 7 atoms" << endl;
			cout << "6. 8 atoms" << endl;
			cout << "7. Back" << endl;
			cin >> choice;
			if (choice == 7)
			{
				clear_screen();
				menu(game_board, presumed_positions);
			}
			else
			{
				create_random_game_array(game_board, 8, choice + 2);
				presumed_positions = new string[100];
				initialize_game(game_board, 0, 0, 8, presumed_positions);
			}
		}
		else if (choice == 3)
		{
			clear_screen();
			cout << "\033[33m" << "Choose number of hidden atoms: " << "\033[0m" << endl;
			cout << "1. 3 atoms" << endl;
			cout << "2. 4 atoms" << endl;
			cout << "3. 5 atoms" << endl;
			cout << "4. 6 atoms" << endl;
			cout << "5. 7 atoms" << endl;
			cout << "6. 8 atoms" << endl;
			cout << "7. Back" << endl;
			cin >> choice;
			if (choice == 7)
			{
				clear_screen();
				menu(game_board, presumed_positions);
			}
			else
			{
				create_random_game_array(game_board, 10, choice + 2);
				presumed_positions = new string[144];
				initialize_game(game_board, 0, 0, 10, presumed_positions);
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
		end_of_game(game_board, presumed_positions);
	}
	else
	{
		clear_screen();
		menu(game_board, presumed_positions);
	}
}

void black_box_ascii_art()
{
	cout << "\033[31m" << "______  _               _     ______                _____                         \n" << "\033[0m";
	cout << "\033[38;5;208m" << "| ___ \\| |             | |    | ___ \\              |  __ \\                        \n" << "\033[0m";
	cout << "\033[33m" << "| |_/ /| |  __ _   ___ | | __ | |_/ /  ___  __  __ | |  \\/  __ _  _ __ ___    ___ \n" << "\033[0m";
	cout << "\033[92m" << "| ___ \\| | / _` | / __|| |/ / | ___ \\ / _ \\ \\ \\/ / | | __  / _` || '_ ` _ \\  / _ \\\n" << "\033[0m";
	cout << "\033[34m" << "| |_/ /| || (_| || (__ |   <  | |_/ /| (_) | >  <  | |_\\ \\| (_| || | | | | ||  __/\n" << "\033[0m";
	cout << "\033[38;5;69m" << "\\____/ |_| \\__,_| \\___||_|\\_\\ \\____/  \\___/ /_/\\_\\  \\____/ \\__,_||_| |_| |_| \\___|\n" << "\033[0m";
	cout << "\033[35m" << "        Tomasz Nazar                 s197613                    ACiR_3 \n" << "\033[0m";
	cout << "                                                                                   \n" << "\033[0m";
}

void clear_screen()
{
#if _WIN32
	system("cls");
#else
	system("clear");
#endif
}

void display_controls()
{
	cout << "\033[33m" << "Controls:" << "\033[0m" << endl;
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

void end_of_game(string*& game_board, string*& presumed_positions)
{
	clear_screen();
	cout << "\033[35m" << " _____ _   _______   ___________   _____   ___  ___  ___ _____ " << "\033[0m" << endl;
	cout << "\033[38;5;69m" << "|  ___| \\ | |  _  \\ |  _  |  ___| |  __ \\ / _ \\ |  \\/  ||  ___|" << "\033[0m" << endl;
	cout << "\033[34m" << "| |__ |  \\| | | | | | | | | |_    | |  \\// /_\\ \\| .  . || |__  " << "\033[0m" << endl;
	cout << "\033[92m" << "|  __|| . ` | | | | | | | |  _|   | | __ |  _  || |\\/| ||  __|" << "\033[0m" << endl;
	cout << "\033[33m" << "| |___| |\\  | |/ /  \\ \\_/ / |     | |_\\ \\| | | || |  | || |___" << "\033[0m" << endl;
	cout << "\033[31m" << "\\____/\\_| \\_/___/    \\___/\\_|      \\____/\\_| |_/\\_|  |_/\\____/ " << "\033[0m" << endl;
	delete[] game_board;
	delete[] presumed_positions;
	exit(0);
}