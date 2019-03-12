#include "pch.h"
#include <iostream>
#include <windows.h>
#include "conio.h"
#include <algorithm>
#include <iterator>
#include <regex>

// Define keycodes
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define _WIN32_WINNT 0x0500
#define PLUS 61
#define MINUS 45

//Game intro
void intro();

void print_world(char **size, int rows, int columns);
// Init function - contains the main variables and functions that are essential
void init();
// Console function - contains the variables that are needed to create the game world
int console(int *columns, int *rows);
// Game_Size function - create an array containing game world
void game_size(char **size, int columns, int rows);
// Game function - Game logic
void game(char **size, int columns, int rows);
// Create_player function - Creates the character and sets it's pivot
void create_player(char **size, int columns, int rows);
// Fontsize function - changes font size of console.
void fontsize();
// Clear function - clears console. :v
void clear();

int main()
{
	init();

	int columns, rows;
	columns = 0;
	rows = 0;

	int font_width = 8;
	int font_height = 8;

	console(&columns, &rows);

	// Create dynamic 2D array
	char** console_size = new char*[rows];

	intro();

	fontsize();

	game_size(console_size, columns, rows);

	create_player(console_size, columns, rows);

	game(console_size, columns, rows);

	return 0;
}

void init() {
	// Make window size static
	// Assigning consoleWindow to variable and making the console heigh and width static
	HWND consoleWindow = GetConsoleWindow();
	SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

	// Remove console scrollbar
	// Assing StdHandle to variable
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	// Assign Screen Buffer Info to variable
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(handle, &info);

	// Set new size of console window
	COORD new_size =
	{
		info.srWindow.Right - info.srWindow.Left + 1,
		info.srWindow.Bottom - info.srWindow.Top + 1
	};
	SetConsoleScreenBufferSize(handle, new_size);
}

int console(int *columns, int *rows) {

	// Create a variable that is able to hold info about console
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	// Passing variables to csbi variable
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	// Assigning values from csbi to variables
	*columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	*rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	// Handle output of console
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	// Get console cursos info
	CONSOLE_CURSOR_INFO cursorInfo;

	// Getting and setting console cursos visibility
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = false; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);

	// Polish chars
	setlocale(LC_ALL, "");

	// Every variable that is needed is changed via pointers, so this function doesn't have to return anything
	return 0;
}

void game_size(char **size, int columns, int rows) {
	// Add columns to existing rows
	for (int i = 0; i < rows; ++i) {
		size[i] = new char[columns];
	}

	// Fill all the array space with dots
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < columns; ++j)
		{
			size[i][j] = ' ';
		}
	}
}

void game(char **size, int columns, int rows) {

	print_world(size, rows, columns);

	// Get output of console
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	// Get font info
	PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx = new CONSOLE_FONT_INFOEX();
	// Get size of font
	lpConsoleCurrentFontEx->cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(out, 0, lpConsoleCurrentFontEx);

	char *tmp_rows = new char[rows]; // Holds shifted row
	char *tmp_columns = new char[columns]; // Holds shifted column

	// Getting keyboard input in real time
	int c = 0;
	char input = ' ';

	// Game loop
	while (1)
	{
		input = ' ';
		c = 0;

		// Read key input
		switch ((c = _getch())) {
		case KEY_UP:
			// Get first up
			for (int i = 0; i < columns; ++i) {
				tmp_columns[i] = size[0][i];
			}
			// Pass from the bottom to top
			for (int j = 0; j < rows-1; j++) {
				for (int k = 0; k < columns; ++k) {
					size[j][k] = size[j + 1][k];
				}
			}
			// Put at last bottom
			for (int i = 0; i < columns; ++i) {
				size[rows-1][i] = tmp_columns[i];
			}
			clear();
			print_world(size, rows, columns);
			break;
		case KEY_DOWN:
			// Get last bottom
			for (int i = 0; i < columns; ++i) {
				tmp_columns[i] = size[rows-1][i];
			}
			// Pass from the top to bottom
			for (int j = rows-1; j > 0; j--) {
				for (int k = columns-1; k > -1; --k) {
					size[j][k] = size[j-1][k];
				}
			}
			// Put at first top
			for (int i = 0; i < columns; ++i) {
				size[0][i] = tmp_columns[i];
			}
			clear();
			print_world(size, rows, columns);
			break;
		case KEY_LEFT:
			// Get first left
			for (int i = 0; i < rows; ++i) {
				tmp_columns[i] = size[i][0];
			}
			// Pass from the beggining to the end
			for (int j = 0; j < columns; j++) {
				for (int k = 0; k < rows; ++k) {
					size[k][j] = size[k][j + 1];
				}
			}
			// Put at the last right
			for (int i = 0; i < rows; ++i) {
				size[i][columns - 1] = tmp_columns[i];
			}
			clear();
			print_world(size, rows, columns);
			break;

		case KEY_RIGHT:
			// Get last right
			for (int i = 0; i < rows; ++i) {
				tmp_columns[i] = size[i][columns-1];
			}
			// Pass from the end to the beggining
			for (int j = columns-1; j > 0; j--) {
				for (int k = rows-1; k > -1; --k) {
					size[k][j] = size[k][j - 1];
				}
			}
			// Put at first left
			for (int i = 0; i < rows; ++i) {
				size[i][0] = tmp_columns[i];
			}
			clear();
			print_world(size, rows, columns);
			break;

		case PLUS:
			// Add 1 to width and height font size variables
			lpConsoleCurrentFontEx->dwFontSize.X += 1;
			lpConsoleCurrentFontEx->dwFontSize.Y += 1;
			SetCurrentConsoleFontEx(out, 0, lpConsoleCurrentFontEx);
			clear();
			print_world(size, rows, columns);
			break;

		case MINUS:
			// Substract 1 to width and height font size variables
			lpConsoleCurrentFontEx->dwFontSize.X -= 1;
			lpConsoleCurrentFontEx->dwFontSize.Y -= 1;
			SetCurrentConsoleFontEx(out, 0, lpConsoleCurrentFontEx);
			clear();
			print_world(size, rows, columns);
			break;
		}

	}
}

void clear() {

	// Get size and buffer of cosole
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	// Assign Buffer Info to console
	GetConsoleScreenBufferInfo(console, &screen);

	// Filling the console with white space
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);

	// Set Cursor to the start
	SetConsoleCursorPosition(console, topLeft);
}

void intro() {
	std::cout << "--------------------------------------------------\n";
	std::cout << "-- Witaj w moim pierwszym prostym programie C++ --\n";
	std::cout << "------- ( Naciśnij Enter aby kontynuować ) -------\n";
	_getch();
	clear();
	std::cout << "--- Jest to prosta aplikacja do poruszania się ---\n";
	std::cout << "---------- Wybranym przez Ciebie znakiem ---------\n";
	std::cout << "------- ( Naciśnij Enter aby kontynuować ) -------\n";
	_getch();
	clear();
	std::cout << "------------------ Możliwości: -------------------\n";
	std::cout << "- Strzałki: Poruszanie się, +-: Zwiększ/Zmniejsz -\n";
	std::cout << "------- ( Naciśnij Enter aby kontynuować ) -------\n";
	_getch();
	clear();
}

void print_world(char **size, int rows, int columns) {
	// Print game world
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < columns; ++j)
		{
			std::cout << size[i][j];
		}
	}
}

void create_player(char **size, int columns, int rows) {

	// Init needed variables
	int char_size = 0;
	char char_char = 'X';

	// Set character letter
	std::cout << "Podaj znak z którego będzie stworzona twoja postać: ";
	std::cin >> char_char;

	// Set character length
	std::cout << "Podaj wielkość twojej postaci: ";
	std::cin >> char_size;

	if (!std::cin.fail()) {

		char_size = char_size - 1;

		// Set the middle of the character
		size[char_size][char_size] = char_char;

		// Spawn character
		for (int c = 0; c <= char_size; c++) {
			size[char_size - c][char_size + c] = char_char;
			size[char_size + c][char_size - c] = char_char;
			size[char_size + c][char_size + c] = char_char;
			size[char_size - c][char_size - c] = char_char;
		}

	}
	// If you can't type in numbers... The program exits.
	else {
		std::cout << "No sorka. Nie grasz według zasad i podajesz złe liczby... Nie gramy tak, papatki.";
		_getch();
		exit(0);
	}

}

void fontsize() {

	// Get output of console
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	// Get font info
	PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx = new CONSOLE_FONT_INFOEX();
	// Get size of font
	lpConsoleCurrentFontEx->cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(out, 0, lpConsoleCurrentFontEx);

	// Init variables
	int font_width = 8;
	int font_height = 8;

	// Set font width
	std::cout << "Podaj szerokość czcionki: ";
	std::cin >> font_width;

	if (!std::cin.fail()) {

		// Set font height
		std::cout << "Podaj wysokość czcionki: " << std::endl;
		std::cin >> font_height;

		if (!std::cin.fail()) {
			// Set font size
			lpConsoleCurrentFontEx->dwFontSize.X = font_width;
			lpConsoleCurrentFontEx->dwFontSize.Y = font_height;
			SetCurrentConsoleFontEx(out, 0, lpConsoleCurrentFontEx);
		}
	}
}