#ifndef VISUALT_EXAMPLES_GETCH_H_
#define VISUALT_EXAMPLES_GETCH_H_

#include <stdio.h>
#include <termios.h>
#include <unistd.h>

char getch() {
	struct termios old, new;
	int c;

	tcgetattr(STDIN_FILENO, &old);
	new = old;
	new.c_lflag &= ~(ICANON|ECHO); // NOLINT(hicpp-signed-bitwise)
	tcsetattr(STDIN_FILENO, TCSANOW, &new);
	c = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &old);
	return (char)c;
}

#endif //VISUALT_EXAMPLES_GETCH_H_
