#include <stdio.h>
#include <time.h>
#include "visualt/visualt.h"
#include "getch.h"

typedef struct Canvas Canvas;
typedef struct Obj Obj;

#define rewindScreen() fputs("\033[0;0H", stdout)

int main() {
	Obj canvas, cat, text;

	initializeBlankObj(&canvas, 1, LTSIZES{{100, 30}});
	initializeFileObj(&cat, "res/fallingCat.obj");
	initializeStringObj(&text, 1, LTSTRS{"press a button to make a cat land"});

	while(1) {
		render(&canvas, 1, LTOBJS{&text});
		rewindScreen();
		print(&canvas, true);
		if(getch() == 'q') {
			break;
		}
		for(unsigned int i = 0; i < 6; i++) {
			render(&canvas, 1, LTOBJS{&cat});
			rewindScreen();
			print(&canvas, true);
			nextSprite(&cat);
			nanosleep(&(struct timespec){.tv_sec=0, .tv_nsec=1e+8}, NULL);
		}
	}

	releaseObjs(3, LTOBJS{&canvas, &cat, &text});

	return 0;
}
