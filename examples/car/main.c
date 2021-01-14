#include <time.h>
#include <stdlib.h>
#include "visualt/visualt.h"
#include "utilities.h"

#define DELAY_DURATION 100
#define PRINT_DURATION 50

double const unit = 10; // m
double const mass = 1000; // kg
double const traction = 600; // N
double const cDrag = 2;
double const crr = 20;
double const cJump = 3;
double const g = 9.80665; // m/s^2

typedef struct {
	double wheelState, maxSpeed;
	unsigned short terrainState;
	bool accelerating;
} AnimationStates;

double const dT = (DELAY_DURATION+PRINT_DURATION)/1000.;
double xVelocity = 0, yVelocity = 0;
AnimationStates as = {.wheelState=0, .maxSpeed=0, .terrainState=2, .accelerating = false};
VTObj canvasObj, terrainObj, terrainStencilObj, backgroundObj, carObj;
VTObj *const canvas = &canvasObj, *const terrain = &terrainObj, *const terrainStencil = &terrainStencilObj, *const background = &backgroundObj, *const car = &carObj;

int getLastKeyPress(void) {
	int c = 0;
	while(kbhit()) {
		c = getch();
	}
	return c;
}

int unitShift(double const v) {
	return (int)(v/dT/unit);
}

double estimateMaxSpeed(void) {
	double v = 0;
	for(int i = 0; i < 60; i++) {
		double fDrag = -cDrag*v*v;
		double fRR = -crr*v;
		v = v+(traction+fDrag+fRR)/mass;
	}
	return v;
}

void generateTerrain(int distance) {
	vtShift(terrain, VT_LEFT, distance);
	for(int x = vtExtremum(terrain, VT_RIGHT)-distance+1; x <= vtExtremum(terrain, VT_RIGHT); x++) {
		switch(as.terrainState) {
			default:
			case 0://high
				as.terrainState = rand()%70 ? 0 : 1;
				break;
			case 1://downslope
				as.terrainState = 2;
				break;
			case 2://low
				as.terrainState = rand()%90 ? 2 : 3;
				break;
			case 3://upslope
				as.terrainState = 0;
				break;
		}
		vtSetSprite(terrainStencil, as.terrainState);
		vtGotoX(NULL, terrainStencil, x);
		vtStamp(terrain, 1, LTOBJS{terrainStencil});
	}
}

void terrainAnimation(void) {
	generateTerrain(unitShift(xVelocity));
}

void backgroundAnimation() {
	int shift = unitShift(xVelocity/2);
	if(unitShift(xVelocity) >= 1 && shift < 1) {
		shift = 1;
	}
	vtRotate(background, VT_LEFT, shift);
}

void carAnimation(void) {
	as.wheelState += (double)unitShift(xVelocity)/(double)unitShift(as.maxSpeed);
	if(as.wheelState >= 4) {
		as.wheelState -= 4;
	}
	vtSetSprite(car, (int)as.wheelState);

	vtChangeY(NULL, car, unitShift(yVelocity));
	yVelocity = yVelocity+dT*-g;
	if(vtIsTouching(terrain, car, 1, LTOBJS{terrain})) {
		yVelocity = 0;
		if(vtIsTouchingGlyph(terrain, car, vtChar("/"))) {
			yVelocity = dT*xVelocity*cJump;
		}
		do {
			vtChangeY(NULL, car, 1);
		} while(vtIsTouching(terrain, car, 1, LTOBJS{terrain}));
	}

	if(vtYPosition(car) > -2) {
		as.accelerating = false;
	}
	// Fdrag = - Cdrag * v * |v|
	double fDrag = -cDrag*xVelocity*xVelocity;
	// Frr = - Crr * v
	double fRR = -crr*xVelocity;
	xVelocity = xVelocity+dT*((as.accelerating ? traction : 0)+fDrag+fRR)/mass;
}

void redraw(void) {
	vtRender(canvas, 3, LTOBJS{background, terrain, car});
	// vtDrawAxes(canvas);
	rewindCursor();
	hideCursor();
	vtPrint(canvas, true);
	showCursor();
	printf("xVelocity=%f\nyVelocity=%f\nkm/h=%f\nestMaxSpeed=%f\n", xVelocity, yVelocity, xVelocity*3.6, as.maxSpeed);
}

void initObjs(void) {
	vtInitializeBlank(canvas, 1, LTSIZES{81, 15});
	vtInitializeBlank(terrain, 1, LTSIZES{81, 15});
	vtInitializeStrings(terrainStencil, 4, LTSTRS{"█\n█\n▓\n▒", "\\\n█\n▓\n▒", "\v\n█\n▓\n▒", "/\n█\n▓\n▒"});
	vtGotoY(NULL, terrainStencil, -6);
	vtInitializeStrings(background, 1, LTSTRS{"\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v··\n\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v····\n\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v···\v\v\v\v\v\v\v\v··\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v····\n\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v··\v\v··\v\v····\v\v\v\v\v\v····\v\v·\v\v\v\v··\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v··\v\v\v\v\v\v\v\v\v\v······\n\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v··\v\v········\v\v\v\v\v\v····\v\v··\v\v\v··\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v····················\n\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v····\v·········\v\v\v\v·········\v\v\v···\v\v··\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v··\v\v\v······················\n\v\v\v\v··\v\v\v\v·····\v··············\v\v\v·····················\v\v\v\v\v\v\v\v\v\v\v\v\v····························\n\v\v·····\v\v······················\v·························\v\v\v\v\v\v\v\v\v······························\n··························································\v\v\v\v\v\v\v·················\v·················\n························································\v··\v\v\v\v·····································\n····································································································"});
	vtGotoY(NULL, background, 1);
	vtInitializeStrings(car, 4, LTSTRS{"\v\v\v____\n__/  [_\\___\n=─◓─────◓─\"", "\v\v\v____\n__/  [_\\___\n=─◑─────◑─\"", "\v\v\v____\n__/  [_\\___\n=─◒─────◒─\"", "\v\v\v____\n__/  [_\\___\n=─◐─────◐─\""});
	vtGotoXY(NULL, car, -30, -7);
}

int main(void) {
	srand(time(NULL));
	as.maxSpeed = estimateMaxSpeed();
	initObjs();
	generateTerrain(vtWidth(terrain));
	conioTerminalMode();

	while(true) {
		backgroundAnimation();
		terrainAnimation();
		carAnimation();
		redraw();
		int input = getLastKeyPress();
		printf(">%c", input);
		switch(input) {
			default:
				as.accelerating = false;
				break;
			case 'C':
			case 'd':
				as.accelerating = true;
				break;
			case 'q':
				vtRelease(3, LTOBJS{canvas, terrain, terrainStencil});
				return 0;
		}

		msleep(DELAY_DURATION);
	}
}