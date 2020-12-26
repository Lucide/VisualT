#include <windows.h>
#include <signal.h>
#include <stdio.h>
#include "utilities.h"

static DWORD original = 0;

static BOOL WINAPI HandlerRoutine(_In_ DWORD dwCtrlType) {
	switch(dwCtrlType) {
		case CTRL_C_EVENT:
			exit(0);
		default:
			// Pass signal on to the next handler
			return FALSE;
	}
}

void resetTerminal(void) {
	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), original);
	clearScreen();
}

void conioTerminalMode(void) {
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(hStdin, &original);
	atexit(resetTerminal);
	SetConsoleCtrlHandler(HandlerRoutine, TRUE);
	SetConsoleMode(hStdin, original&(~ENABLE_ECHO_INPUT));
}

void rewindCursor(void) {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), (COORD){0, 0});
}

void showCursor(void) {
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &(CONSOLE_CURSOR_INFO){.dwSize=100, .bVisible=TRUE});
}

void hideCursor(void) {
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &(CONSOLE_CURSOR_INFO){.dwSize=100, .bVisible=FALSE});
}

void clearScreen(void) {
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	SMALL_RECT scrollRect;
	COORD scrollTarget;
	CHAR_INFO fill;
	// Get the number of character cells in the current buffer.
	if(!GetConsoleScreenBufferInfo(hStdout, &csbi)) {
		return;
	}
	// Scroll the rectangle of the entire buffer.
	scrollRect.Left = 0;
	scrollRect.Top = 0;
	scrollRect.Right = csbi.dwSize.X;
	scrollRect.Bottom = csbi.dwSize.Y;
	// Scroll it upwards off the top of the buffer with a magnitude of the entire height.
	scrollTarget.X = 0;
	scrollTarget.Y = (SHORT)(0-csbi.dwSize.Y);
	// Fill with empty spaces with the buffer's default text attribute.
	fill.Char.UnicodeChar = TEXT(' ');
	fill.Attributes = csbi.wAttributes;
	// Do the scroll
	ScrollConsoleScreenBuffer(hStdout, &scrollRect, NULL, scrollTarget, &fill);
	// Move the cursor to the top left corner too.
	csbi.dwCursorPosition.X = 0;
	csbi.dwCursorPosition.Y = 0;
	SetConsoleCursorPosition(hStdout, csbi.dwCursorPosition);
}

int msleep(unsigned long msec) {
	Sleep(msec);
	return 0;
}