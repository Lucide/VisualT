#pragma once
#ifndef VTEXAMPLES_UTILITIES_H
#define VTEXAMPLES_UTILITIES_H

void resetTerminal();

void conioTerminalMode();

int getch();

int kbhit();

void rewindCursor();

void showCursor(void);

void hideCursor(void);

void clearScreen();

int msleep(unsigned long msec);

#endif //VTEXAMPLES_UTILITIES_H