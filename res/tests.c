#include "visualt/visualt.h"
#include <stdio.h>
#include <stdalign.h>
#include <stdlib.h>
#include <string.h>

typedef struct CharMap CharMap;
typedef struct Canvas Canvas;
typedef struct Obj Obj;

void penTest(const char penChar[const]) {
	puts("pen stroke test:");
	Obj canvas, p;
	initializeBlankObj(&canvas, 1, LTSIZES{{100, 20}});
	initializeStringObj(&p, 1, LTSTRS{"p"});
	setPenChar(&p, LTCHAR penChar);

	gotoXY(NULL, &p, -57, 7);
	for(unsigned int penSize = 0; penSize <= 5; penSize++) {
		changeX(NULL, &p, 13);
		setPenSize(&p, penSize);
		changeX(&canvas, &p, 0);
	}

	gotoXY(NULL, &p, -46, -2);
	for(unsigned int i = 0; i <= 13; i++) {
		setPenSize(&p, (unsigned short)((double)i/11*5));
		changeX(&canvas, &p, 5);
	}

	gotoXY(NULL, &p, 40, 7);
	for(unsigned int i = 0; i <= 7; i++) {
		setPenSize(&p, (unsigned short)(short)((double)i/7*5));
		changeY(&canvas, &p, -2);
	}

	// render(&canvas, 0, NULL);
	print(&canvas, true);
	releaseObjs(2, LTOBJS{&canvas, &p});
	puts("ok");
}

void alignTest() {
	puts("align test:");
	Obj canvas, target, viewfinder;
	initializeBlankObj(&canvas, 1, LTSIZES{{60, 10}});
	initializeStringObj(&target, 1, LTSTRS{"0"});
	initializeStringObj(&viewfinder, 1, LTSTRS{"██\n██"});

	// top left 2x2
	gotoXY(NULL, &viewfinder, -28, 3);
	gotoXY(NULL, &target, -28, 3);
	align(&viewfinder, 0);
	stamp(&canvas, 2, LTOBJS{&viewfinder, &target});

	// top right 2x2
	gotoXY(NULL, &viewfinder, -23, 3);
	gotoXY(NULL, &target, -23, 3);
	align(&viewfinder, 1);
	stamp(&canvas, 2, LTOBJS{&viewfinder, &target});

	// bottom left 2x2
	gotoXY(NULL, &viewfinder, -18, 3);
	gotoXY(NULL, &target, -18, 3);
	align(&viewfinder, 2);
	stamp(&canvas, 2, LTOBJS{&viewfinder, &target});

	// bottom right 2x2
	gotoXY(NULL, &viewfinder, -12, 3);
	gotoXY(NULL, &target, -12, 3);
	align(&viewfinder, 3);
	stamp(&canvas, 2, LTOBJS{&viewfinder, &target});

	setText(&viewfinder, LTSTR "███\n███");

	// top left 3x2
	gotoXY(NULL, &viewfinder, -7, 3);
	gotoXY(NULL, &target, -7, 3);
	align(&viewfinder, 0);
	stamp(&canvas, 2, LTOBJS{&viewfinder, &target});

	// top right 3x2
	gotoXY(NULL, &viewfinder, -1, 3);
	gotoXY(NULL, &target, -1, 3);
	align(&viewfinder, 1);
	stamp(&canvas, 2, LTOBJS{&viewfinder, &target});

	// bottom left 3x2
	gotoXY(NULL, &viewfinder, 5, 3);
	gotoXY(NULL, &target, 5, 3);
	align(&viewfinder, 2);
	stamp(&canvas, 2, LTOBJS{&viewfinder, &target});

	// bottom right 3x2
	gotoXY(NULL, &viewfinder, 11, 3);
	gotoXY(NULL, &target, 11, 3);
	align(&viewfinder, 3);
	stamp(&canvas, 2, LTOBJS{&viewfinder, &target});

	setText(&viewfinder, LTSTR "██\n██\n██");

	// top left 2x3
	gotoXY(NULL, &viewfinder, -28, -1);
	gotoXY(NULL, &target, -28, -1);
	align(&viewfinder, 0);
	stamp(&canvas, 2, LTOBJS{&viewfinder, &target});

	// top right 2x3
	gotoXY(NULL, &viewfinder, -23, -1);
	gotoXY(NULL, &target, -23, -1);
	align(&viewfinder, 1);
	stamp(&canvas, 2, LTOBJS{&viewfinder, &target});

	// bottom left 2x3
	gotoXY(NULL, &viewfinder, -18, -1);
	gotoXY(NULL, &target, -18, -1);
	align(&viewfinder, 2);
	stamp(&canvas, 2, LTOBJS{&viewfinder, &target});

	// bottom right 2x3
	gotoXY(NULL, &viewfinder, -12, -1);
	gotoXY(NULL, &target, -12, -1);
	align(&viewfinder, 3);
	stamp(&canvas, 2, LTOBJS{&viewfinder, &target});

	setText(&viewfinder, LTSTR "███\n███\n███");

	// top left 3x3
	gotoXY(NULL, &viewfinder, -7, -1);
	gotoXY(NULL, &target, -7, -1);
	align(&viewfinder, 0);
	stamp(&canvas, 2, LTOBJS{&viewfinder, &target});

	// top right 3x3
	gotoXY(NULL, &viewfinder, -1, -1);
	gotoXY(NULL, &target, -1, -1);
	align(&viewfinder, 1);
	stamp(&canvas, 2, LTOBJS{&viewfinder, &target});

	// bottom left 3x3
	gotoXY(NULL, &viewfinder, 5, -1);
	gotoXY(NULL, &target, 5, -1);
	align(&viewfinder, 2);
	stamp(&canvas, 2, LTOBJS{&viewfinder, &target});

	// bottom right 3x3
	gotoXY(NULL, &viewfinder, 11, -1);
	gotoXY(NULL, &target, 11, -1);
	align(&viewfinder, 3);
	stamp(&canvas, 2, LTOBJS{&viewfinder, &target});

	print(&canvas, true);

	releaseObjs(3, LTOBJS{&viewfinder, &target, &canvas});
	puts("ok");
}

void drawToStringTest() {
	puts("draw to string test:");
	Obj canvas, a;
	unsigned int length;
	unsigned char *s;
	initializeBlankObj(&canvas, 1, LTSIZES{{60, 16}});
	initializeStringObj(&a, 1, LTSTRS{"▀▄─▀▄─▀▄─▀▄─▀▄─▀▄─▀▄─▀\n▄▀─▄▀─▄▀─▄▀─▄▀─▄▀─▄▀─▄\n▀─▄▀─▄▀─▄▀─▄▀─▄▀─▄▀─▄▀\n▄─▀▄─▀▄─▀▄─▀▄─▀▄─▀▄─▀▄\n▀▄─▀▄─▀▄─▀▄─▀▄─▀▄─▀▄─▀\n▄▀─▄▀─▄▀─▄▀─▄▀"});

	stamp(&canvas, 1, LTOBJS{&a});
	length = printToString(&canvas, true, &s);
	printf("length: %d\n%s\n", length, s);
	free(s);
	length = printToString(&canvas, false, &s);
	printf("length: %d\n%s\n", length, s);
	free(s);

	releaseObjs(2, LTOBJS{&a, &canvas});
	puts("ok");
}

void collisionTest() {
	puts("collision test:");
	Obj canvas, penLayer, pot, circle;

	initializeBlankObj(&canvas, 1, LTSIZES{{60, 16}});
	initializeObjObj(&penLayer, &canvas);
	initializeStringObj(&pot, 1, LTSTRS{"|░░░░|\n\\░░░░/"});
	initializeStringObj(&circle, 1, LTSTRS{"\v\v\v____\n\v.'\v\v\v\v`.\n/\v\v\v\v\v\v\v\v\\\n|\v\v\v\v\v\v\v\v|\n\\\v\v\v\v\v\v\v\v/\n\v`.____.'"});

	setPenSize(&pot, 2);
	setPenChar(&pot, LTCHAR "░");
	gotoXY(NULL, &pot, 18, 6);
	gotoXY(&penLayer, &pot, 18, -6);

	render(&canvas, 3, LTOBJS{&penLayer, &pot, &circle});
	print(&canvas, true);
	printf("isTouching(&canvas, &circle, 1, VTOBJS{&pot})=%d\n", isTouching(&canvas, &circle, 1, LTOBJS{&pot}));
	printf("isTouchingChar(&penLayer, &circle, VTCHAR \"░\")=%d\n", isTouchingChar(&penLayer, &circle, LTCHAR "░"));
	printf("isOutside(&canvas, &circle)=%d\n", isOutside(&canvas, &circle));

	gotoX(NULL, &circle, 18);

	render(&canvas, 3, LTOBJS{&penLayer, &pot, &circle});
	print(&canvas, true);
	printf("isTouching(&canvas, &circle, 1, VTOBJS{&pot})=%d\n", isTouching(&canvas, &circle, 1, LTOBJS{&pot}));
	printf("isTouchingChar(&penLayer, &circle, VTCHAR \"░\")=%d\n", isTouchingChar(&penLayer, &circle, LTCHAR "░"));
	printf("isOutside(&canvas, &circle)=%d\n", isOutside(&canvas, &circle));

	gotoY(NULL, &circle, -5);

	render(&canvas, 3, LTOBJS{&penLayer, &pot, &circle});
	print(&canvas, true);
	printf("isTouching(&canvas, &circle, 1, VTOBJS{&pot})=%d\n", isTouching(&canvas, &circle, 1, LTOBJS{&pot}));
	printf("isTouchingChar(&penLayer, &circle, VTCHAR \"░\")=%d\n", isTouchingChar(&penLayer, &circle, LTCHAR "░"));
	printf("isOutside(&canvas, &circle)=%d\n", isOutside(&canvas, &circle));

	changeY(NULL, &circle, 1);

	render(&canvas, 3, LTOBJS{&penLayer, &pot, &circle});
	print(&canvas, true);
	printf("isTouching(&canvas, &circle, 1, VTOBJS{&pot})=%d\n", isTouching(&canvas, &circle, 1, LTOBJS{&pot}));
	printf("isTouchingChar(&penLayer, &circle, VTCHAR \"░\")=%d\n", isTouchingChar(&penLayer, &circle, LTCHAR "░"));
	printf("isOutside(&canvas, &circle)=%d\n", isOutside(&canvas, &circle));

	clear(&penLayer);

	render(&canvas, 3, LTOBJS{&penLayer, &pot, &circle});
	print(&canvas, true);
	printf("isTouching(&canvas, &circle, 1, VTOBJS{&pot})=%d\n", isTouching(&canvas, &circle, 1, LTOBJS{&pot}));
	printf("isTouchingChar(&penLayer, &circle, VTCHAR \"░\")=%d\n", isTouchingChar(&penLayer, &circle, LTCHAR "░"));
	printf("isOutside(&canvas, &circle)=%d\n", isOutside(&canvas, &circle));

	changeY(NULL, &circle, -4);

	render(&canvas, 3, LTOBJS{&penLayer, &pot, &circle});
	print(&canvas, true);
	printf("isTouching(&canvas, &circle, 1, VTOBJS{&pot})=%d\n", isTouching(&canvas, &circle, 1, LTOBJS{&pot}));
	printf("isTouchingChar(&penLayer, &circle, VTCHAR \"░\")=%d\n", isTouchingChar(&penLayer, &circle, LTCHAR "░"));
	printf("isOutside(&canvas, &circle)=%d\n", isOutside(&canvas, &circle));
	putchar('\n');
	printf("isTouching(&canvas, &circle, 1, VTOBJS{&circle})=%d\n", isTouching(&canvas, &circle, 1, LTOBJS{&circle}));

	releaseObjs(4, LTOBJS{&pot, &circle, &penLayer, &canvas});
	puts("ok");
}

void cloneResizeTest() {
	puts("clone resize test:");
	Obj canvas, a;
	initializeBlankObj(&canvas, 2, LTSIZES{{60, 10},
																				 {1,  1}});
	initializeStringObj(&a, 2, LTSTRS{"║█║█║║█║█║█║║█║█║║█║█║█\n║█║█║║█║█║█║║█║█║║█║█║█\n║║║║║║║║║║║║║║║║║║║║║║║\n╚╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩", "\v\v\v\v\v\v\v\v\v\v\v\v╭━━━\n\v\v\v╭━━╮\v\v\v\v\v┃RAWR\n\v\v╭╯┊◣╰━━━╮\v╰┳━━\n\v\v┃┊┊┊╱▽▽▽┛\v\v┃\v\v\n\v\v┃┊┊┊▏━━━━━━╯\v\v\n━━╯┊┊┊╲△△△┓\v\v\v\v\v\n┊┊┊┊╭━━━━━╯\v\v\v\v\v"});

	print(&canvas, true);
	setSprite(&canvas, 1);
	print(&canvas, true);
	setSprite(&canvas, 0);

	resize(&canvas,40,6);
	render(&canvas, 1, LTOBJS{&a});
	cloneSprite(&canvas, 1, &canvas, 0);
	setSprite(&a, 1);
	render(&canvas, 1, LTOBJS{&a});

	print(&canvas, true);
	setSprite(&canvas, 1);
	print(&canvas, true);

	releaseObjs(2, LTOBJS{&a, &canvas});
	puts("ok");
}

void setSpriteTextTest() {
	puts("setSpriteText test:");
	Obj a;
	initializeStringObj(&a, 1, LTSTRS{"║█║█║║█║█║█║║█║█║║█║█║█\n║█║█║║█║█║█║║█║█║║█║█║█\n║║║║║║║║║║║║║║║║║║║║║║║\n╚╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩"});
	print(&a, true);
	setText(&a, LTSTR "\v\v\v\v\v\v\v\v\v\v\v\v╭━━━\n\v\v\v╭━━╮\v\v\v\v\v┃RAWR\n\v\v╭╯┊◣╰━━━╮\v╰┳━━\n\v\v┃┊┊┊╱▽▽▽┛\v\v┃\v\v\n\v\v┃┊┊┊▏━━━━━━╯\v\v\n━━╯┊┊┊╲△△△┓\v\v\v\v\v\n┊┊┊┊╭━━━━━╯\v\v\v\v\v");
	print(&a, true);

	releaseObjs(1, LTOBJS{&a});
	puts("ok");
}

void dynamicMemoryTest() {
	puts("dynamic memory test:");

	puts("1-initializeBlankObj() with dynamic array of pointer to (dynamic) unsigned int[2]");
	{
		Obj a;
		unsigned int (*v)[2] = malloc(2*sizeof(unsigned int (*)[2]));
		v[0][0] = 1;
		v[0][1] = 2;
		v[1][0] = 2;
		v[1][1] = 1;
		initializeBlankObj(&a, 2, (VTSizes)v);
		free(v);
		print(&a, true);
		nextSprite(&a);
		print(&a, true);
		releaseObjs(1, LTOBJS{&a});
	}

	puts("2-initializeArrayObj() with dynamic array of unsigned int");
	{
		Obj a;
		VTChar *v = malloc(4*sizeof(VTChar));
		v[0] = 1;
		v[1] = 1;
		v[2] = 1;
		v[3] = 0;
		initializeArrayObj(&a, v);
		free(v);
		print(&a, true);
		releaseObjs(1, LTOBJS{&a});
	}

	puts("3-initializeStringObj() with dynamic array of pointer to (dynamic) char");
	{
		Obj a;
		char **strings = malloc(2*sizeof(uint8_t *));
		strings[0] = malloc(6*sizeof(char));
		strcpy((char *)strings[0], "Hello");
		strings[1] = malloc(7*sizeof(char));
		strcpy((char *)strings[1], "World!");
		initializeStringObj(&a, 2, (VTStrs)strings);
		free(strings[0]);
		free(strings[1]);
		free(strings);
		print(&a, true);
		nextSprite(&a);
		print(&a, true);
		releaseObjs(1, LTOBJS{&a});
	}

	puts("4-fill() with dynamic array of char");
	{
		char *c = malloc(4*sizeof(char));
		strcpy(c, "▀");
		Obj a;
		initializeBlankObj(&a, 1, LTSIZES{{2, 2}});
		fill(&a, LTCHAR c);
		free(c);
		print(&a, true);
		releaseObjs(1, LTOBJS{&a});
	}

	puts("5-initializeStringObj(), setY(), render() with dynamic array of pointer to (dynamic) Obj");
	{
		Obj canvas;
		initializeBlankObj(&canvas, 1, LTSIZES{{10, 5}});
		Obj **objs = malloc(2*sizeof(Obj *));
		objs[0] = malloc(sizeof(Obj));
		initializeStringObj(objs[0], 1, LTSTRS{"Hello"});
		gotoY(NULL, objs[0], 1);
		objs[1] = malloc(sizeof(Obj));
		initializeStringObj(objs[1], 1, LTSTRS{"World!"});
		gotoY(NULL, objs[1], -1);

		render(&canvas, 2, (VTObjs)objs);
		print(&canvas, true);

		releaseObjs(3, LTOBJS{&canvas, objs[0], objs[1]});
		free(objs[0]);
		free(objs[1]);
		free(objs);
	}

	puts("ok");
}

int main() {
	about();
	puts("\nThese tests are meant to be ran with Valgrind: https://valgrind.org/\n");

	printf("sizeof(void*)=%lu alignof(void*)=%lu\n", sizeof(void *), alignof(void *));
	printf("sizeof(int)=%lu alignof(int)=%lu\n", sizeof(int), alignof(int));
	printf("sizeof(short)=%lu alignof(short)=%lu\n", sizeof(short), alignof(short));
	printf("sizeof(bool)=%lu alignof(bool)=%lu\n", sizeof(bool), alignof(bool));
	putchar('\n');
	printf("sizeof(CharMap)=%lu alignof(CharMap)=%lu\n", sizeof(CharMap), alignof(CharMap));
	printf("sizeof(Obj)=%lu alignof(Obj)=%lu\n", sizeof(Obj), alignof(Obj));

	putchar('\n');

	// penTest("█");
	penTest("#");

	alignTest();
	drawToStringTest();
	collisionTest();
	cloneResizeTest();
	setSpriteTextTest();
	dynamicMemoryTest();

	// getchar();
}

