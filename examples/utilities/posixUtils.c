#define _DEFAULT_SOURCE
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>

#undef _DEFAULT_SOURCE
#include "utilities.h"

static struct termios original;

static void HandlerRoutine() {
	exit(0);
}

void resetTerminal(void) {
	tcsetattr(0, TCSANOW, &original);
	clearScreen();
}

void conioTerminalMode(void) {
	tcgetattr(0, &original);
	atexit(resetTerminal);
	signal(SIGINT, HandlerRoutine);
	struct termios raw = original;
	raw.c_lflag &= ~(ECHO|ICANON);
	tcsetattr(0, TCSANOW, &raw);
	// in non-canonical original, we can set whether getc() returns immediately
	// when there is no data, or whether it waits until there is data
	// raw.c_cc[VMIN] = 1;
	// raw.c_cc[VTIME] = 0;
}

int getch(void) {
	int r;
	unsigned char c;
	if((r = read(0, &c, sizeof(c))) < 0) {
		return r;
	} else {
		return c;
	}
}

int kbhit(void) {
	struct timeval tv = {0L, 0L};
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(0, &fds);
	return select(1, &fds, NULL, NULL, &tv);
}

void rewindCursor(void) {
	fputs("\33[0;0H", stdout);
}

void showCursor(void) {
	fputs("\e[?25h", stdout);
}

void hideCursor(void) {
	fputs("\e[?25l", stdout);
}

void clearScreen() {
	fputs("\033c", stdout);
}

int msleep(unsigned long msec) {
	struct timespec ts;
	int res;
	ts.tv_sec = msec/1000;
	ts.tv_nsec = (msec%1000)*1000000;
	do {
		res = nanosleep(&ts, &ts);
	} while(res && errno == EINTR);
	return res;
}