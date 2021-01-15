#include <time.h>
#include <math.h>
#include "visualt/visualt.h"
#include "utilities.h"

#define M_PI 3.14159265358979323846264338327

void delay(unsigned int milliseconds) {
	unsigned long pause = milliseconds*(CLOCKS_PER_SEC/1000);
	clock_t now, then;
	now = then = clock();
	while((now-then) < pause) {
		now = clock();
	}
}

void refresh(VTObj const canvas) {
	rewindCursor();
	vtPrint(&canvas, false);
}

void animate(VTObj const canvas, VTObj obj) {
	for(int i = 0; i < 200; ++i) {
		vtClear(&canvas);
		double freq;
		for(int x = -26; x <= 26; ++x) {
			if(i < 100) {
				freq = i/99.*(.2-.1)+.1;
			} else {
				freq = (199-i)/99.*(.2-.1)+.1;
			}
			double amp = sin(i/199.*2.*M_PI)*5.;
			double penPhase = i/199.*2.*M_PI;
			double penSize = (sin(x*freq-penPhase)+1.)/2.*5.+1.;
			double y = sin(x*freq)*amp;
			vtSetPenSize(&obj, (unsigned short)penSize);
			vtGotoXY(NULL, &obj, x, (int)y);
			vtChangeX(&canvas, &obj, 0);
		}
		refresh(canvas);
		delay(10);
	}
}

int main(void) {
	puts("\033c");
	VTObj obj, canvas;
	vtInitializeBlank(&canvas, 1, LTSIZES{{51, 15}});
	vtInitializeStrings(&obj, 1, LTSTRS{"☒"});
	vtSetPenGlyph(&obj, vtChar("≈"));

	while(true){
		animate(canvas, obj);
	}
}

