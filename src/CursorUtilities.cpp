#include <sys/ioctl.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include "CursorUtilities.h"

static struct winsize w;

int getWindowHeight()
{
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	return w.ws_row;
}

int getWindowWidth()
{
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	return w.ws_col;
}

void resizeWindow(int height, int width)
{
	//terminal escape sequence for unix terminals
	std::cout << "\e[8;" << std::to_string(height) << ";" << std::to_string(width) << "t";
}

//sets the cursor position regardless of where the cursor currently is.
//in other words, sets it relative to the terminal indexing
void setAbsoluteCursorPosition(int row, int col) {
	//set absolute position to (0, 0)
	std::cout << "\033[1;0H";
	//move cursor down by <row>
	if (row > 0) {
		std::cout << "\033[" + std::to_string(row) + "B";
	}
	//move cursor right by <col>
	if (col > 0) {
		std::cout << "\033[" + std::to_string(col) + "C";
	}
}

void resetCommandInputCursorPosition(int current_cursor_pos, int ui_buf_height)
{
	//move cursor down the remaining lines to reset buffer
	//for consistent text input area
	std::cout << "\033[" + std::to_string(current_cursor_pos - ui_buf_height) + "N";
}

void clearUserInterfaceBuffer(int height) 
{
	// "\033[2J" clears the screen from top to bottom
	// "\033[<height>;1H" puts the cursor down to row <height> and col 1;
	std::cout << "\033[2J\033[" + std::to_string(height) + ";1H";
}

void moveCursor(int up, int down, int right, int left)
{
	if (up > 0) {
		std::cout << "\033[" + std::to_string(up) + "A";
	}
	if (down > 0) {
		std::cout << "\033[" + std::to_string(down) + "B";
	}
	if (right > 0) {
		std::cout << "\033[" + std::to_string(right) + "C";
	}
	if (left > 0) {
		std::cout << "\033[" + std::to_string(left) + "D";
	}
}

//erases <num_lines> lines above cursor
//TODO:
// 	have this take in a start_from argument. This will require
// 	using information about terminal window and saving cursor pos
void eraseLines(int num_lines)
{
	for (int i = 0; i < num_lines; i++) {
		std::cout << "\033[1A\033[K";
	}
}
