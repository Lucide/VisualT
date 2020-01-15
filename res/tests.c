#include <visualt/visualt.h>
#include <stdio.h>
#include <stdalign.h>
#include <stdlib.h>

typedef struct CharMap CharMap;
typedef struct Canvas Canvas;
typedef struct Obj Obj;

void penTest(const char penChar[const]) {
	puts("pen stroke test:");
	Obj penLayer, p;
	initializeBlankObj(&penLayer, 1, VTSIZES{{100, 20}});
	initializeStringObj(&p, 1, VTSTRS{"p"});
	setPenChar(&p, VTCHAR penChar);

	moveTo(NULL, &p, -57, 7);
	for(unsigned int penSize = 0; penSize <= 5; penSize++) {
		changeX(NULL, &p, 13);
		setPenSize(&p, penSize);
		changeX(&penLayer, &p, 0);
	}

	moveTo(NULL, &p, -46, -2);
	for(unsigned int i = 0; i <= 13; i++) {
		setPenSize(&p, (unsigned short)((double)i/11*5));
		changeX(&penLayer, &p, 5);
	}

	moveTo(NULL, &p, 40, 7);
	for(unsigned int i = 0; i <= 7; i++) {
		setPenSize(&p, (unsigned short)(short)((double)i/7*5));
		changeY(&penLayer, &p, -2);
	}

	// draw(&canvas, 0, NULL);
	print(&penLayer, true);
	deleteObj(&penLayer);
	deleteObj(&p);
	puts("ok");
}

void alignTest() {
	puts("align test:");
	Obj canvas, target, viewfinder;
	initializeBlankObj(&canvas, 1, VTSIZES{{60, 10}});
	initializeStringObj(&target, 1, VTSTRS{"0"});
	initializeStringObj(&viewfinder, 1, VTSTRS{"██\n██"});

	// top left 2x2
	moveTo(NULL, &viewfinder, -28, 3);
	moveTo(NULL, &target, -28, 3);
	align(&viewfinder, 0);
	stamp(&canvas, &viewfinder);
	stamp(&canvas, &target);

	// top right 2x2
	moveTo(NULL, &viewfinder, -23, 3);
	moveTo(NULL, &target, -23, 3);
	align(&viewfinder, 1);
	stamp(&canvas, &viewfinder);
	stamp(&canvas, &target);

	// bottom left 2x2
	moveTo(NULL, &viewfinder, -18, 3);
	moveTo(NULL, &target, -18, 3);
	align(&viewfinder, 2);
	stamp(&canvas, &viewfinder);
	stamp(&canvas, &target);

	// bottom right 2x2
	moveTo(NULL, &viewfinder, -12, 3);
	moveTo(NULL, &target, -12, 3);
	align(&viewfinder, 3);
	stamp(&canvas, &viewfinder);
	stamp(&canvas, &target);

	setSpriteText(&viewfinder, VTSTR "███\n███");

	// top left 3x2
	moveTo(NULL, &viewfinder, -7, 3);
	moveTo(NULL, &target, -7, 3);
	align(&viewfinder, 0);
	stamp(&canvas, &viewfinder);
	stamp(&canvas, &target);

	// top right 3x2
	moveTo(NULL, &viewfinder, -1, 3);
	moveTo(NULL, &target, -1, 3);
	align(&viewfinder, 1);
	stamp(&canvas, &viewfinder);
	stamp(&canvas, &target);

	// bottom left 3x2
	moveTo(NULL, &viewfinder, 5, 3);
	moveTo(NULL, &target, 5, 3);
	align(&viewfinder, 2);
	stamp(&canvas, &viewfinder);
	stamp(&canvas, &target);

	// bottom right 3x2
	moveTo(NULL, &viewfinder, 11, 3);
	moveTo(NULL, &target, 11, 3);
	align(&viewfinder, 3);
	stamp(&canvas, &viewfinder);
	stamp(&canvas, &target);

	setSpriteText(&viewfinder, VTSTR "██\n██\n██");

	// top left 2x3
	moveTo(NULL, &viewfinder, -28, -1);
	moveTo(NULL, &target, -28, -1);
	align(&viewfinder, 0);
	stamp(&canvas, &viewfinder);
	stamp(&canvas, &target);

	// top right 2x3
	moveTo(NULL, &viewfinder, -23, -1);
	moveTo(NULL, &target, -23, -1);
	align(&viewfinder, 1);
	stamp(&canvas, &viewfinder);
	stamp(&canvas, &target);

	// bottom left 2x3
	moveTo(NULL, &viewfinder, -18, -1);
	moveTo(NULL, &target, -18, -1);
	align(&viewfinder, 2);
	stamp(&canvas, &viewfinder);
	stamp(&canvas, &target);

	// bottom right 2x3
	moveTo(NULL, &viewfinder, -12, -1);
	moveTo(NULL, &target, -12, -1);
	align(&viewfinder, 3);
	stamp(&canvas, &viewfinder);
	stamp(&canvas, &target);

	setSpriteText(&viewfinder, VTSTR "███\n███\n███");

	// top left 3x3
	moveTo(NULL, &viewfinder, -7, -1);
	moveTo(NULL, &target, -7, -1);
	align(&viewfinder, 0);
	stamp(&canvas, &viewfinder);
	stamp(&canvas, &target);

	// top right 3x3
	moveTo(NULL, &viewfinder, -1, -1);
	moveTo(NULL, &target, -1, -1);
	align(&viewfinder, 1);
	stamp(&canvas, &viewfinder);
	stamp(&canvas, &target);

	// bottom left 3x3
	moveTo(NULL, &viewfinder, 5, -1);
	moveTo(NULL, &target, 5, -1);
	align(&viewfinder, 2);
	stamp(&canvas, &viewfinder);
	stamp(&canvas, &target);

	// bottom right 3x3
	moveTo(NULL, &viewfinder, 11, -1);
	moveTo(NULL, &target, 11, -1);
	align(&viewfinder, 3);
	stamp(&canvas, &viewfinder);
	stamp(&canvas, &target);

	print(&canvas, true);

	deleteObj(&viewfinder);
	deleteObj(&target);
	deleteObj(&canvas);
	puts("ok");
}

void drawToStringTest() {
	puts("draw to string test:");
	Obj canvas, a, b;
	unsigned int length;
	unsigned char *s;

	initializeBlankObj(&canvas, 2, VTSIZES{{60, 16},
	                                       {60, 16}});
	initializeStringObj(&a, 1, VTSTRS{"▀▄─▀▄─▀▄─▀▄─▀▄─▀▄─▀▄─▀\n▄▀─▄▀─▄▀─▄▀─▄▀─▄▀─▄▀─▄\n▀─▄▀─▄▀─▄▀─▄▀─▄▀─▄▀─▄▀\n▄─▀▄─▀▄─▀▄─▀▄─▀▄─▀▄─▀▄\n▀▄─▀▄─▀▄─▀▄─▀▄─▀▄─▀▄─▀\n▄▀─▄▀─▄▀─▄▀─▄▀"});
	initializeStringObj(&b, 2, VTSTRS{"║█║█║║█║█║█║║█║█║║█║█║█\n║█║█║║█║█║█║║█║█║║█║█║█\n║║║║║║║║║║║║║║║║║║║║║║║\n╚╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩", "\v\v\v\v\v\v\v\v\v\v\v\v╭━━━\n\v\v\v╭━━╮\v\v\v\v\v┃RAWR\n\v\v╭╯┊◣╰━━━╮\v╰┳━━\n\v\v┃┊┊┊╱▽▽▽┛\v\v┃\v\v\n\v\v┃┊┊┊▏━━━━━━╯\v\v\n━━╯┊┊┊╲△△△┓\v\v\v\v\v\n┊┊┊┊╭━━━━━╯\v\v\v\v\v"});
	moveTo(NULL, &a, -10, 3);
	moveTo(NULL, &b, -14, -3);

	setSprite(&canvas, 1);
	stamp(&canvas, &b);
	setSprite(&canvas, 0);

	nextSprite(&b);
	changeX(NULL, &b, 30);

	draw(&canvas, 2, VTOBJS{&a, &b});
	overlay(&canvas, 1, &canvas, 0);
	setSprite(&canvas, 1);

	length = printToString(&canvas, true, &s);
	printf("length: %d\n%s\n", length, s);
	free(s);
	length = printToString(&canvas, false, &s);
	printf("length: %d\n%s\n", length, s);
	free(s);

	deleteObj(&b);
	deleteObj(&a);
	deleteObj(&canvas);
	puts("ok");
}

void collisionTest() {
	puts("collision test:");
	Obj canvas, penLayer, a, b;

	initializeBlankObj(&canvas, 1, VTSIZES{{60, 16}});
	initializeBlankObj(&penLayer, 1, VTSIZES{{60, 16}});
	initializeStringObj(&a, 1, VTSTRS{"|░░░░|\n\\░░░░/"});
	initializeStringObj(&b, 1, VTSTRS{"\v\v\v____\n\v.'\v\v\v\v`.\n/\v\v\v\v\v\v\v\v\\\n|\v\v\v\v\v\v\v\v|\n\\\v\v\v\v\v\v\v\v/\n\v`.____.'"});

	setPenSize(&a, 2);
	setPenChar(&a, VTCHAR "░");
	moveTo(NULL, &a, 18, 6);
	moveTo(&penLayer, &a, 18, -6);

	draw(&canvas, 3, VTOBJS{&penLayer, &a, &b});
	print(&canvas, true);
	printf("isTouching(&canvas, &b, 1, VTOBJS{&a})=%d\n", isTouching(&canvas, &b, 1, VTOBJS{&a}));
	printf("isTouchingChar(&canvas, &b, VTCHAR \"░\")=%d\n", isTouchingChar(&penLayer, &b, VTCHAR "░"));
	printf("isOutside(&canvas,&b)=%d\n", isOutside(&canvas, &b));

	setX(NULL, &b, 18);

	draw(&canvas, 3, VTOBJS{&penLayer,&a, &b});
	print(&canvas, true);
	printf("isTouching(&canvas, &b, 1, VTOBJS{&a})=%d\n", isTouching(&canvas, &b, 1, VTOBJS{&a}));
	printf("isTouchingChar(&canvas, &b, VTCHAR \"░\")=%d\n", isTouchingChar(&penLayer, &b, VTCHAR "░"));
	printf("isOutside(&canvas,&b)=%d\n", isOutside(&canvas, &b));

	setY(NULL, &b, -5);

	draw(&canvas, 3, VTOBJS{&penLayer,&a, &b});
	print(&canvas, true);
	printf("isTouching(&canvas, &b, 1, VTOBJS{&a})=%d\n", isTouching(&canvas, &b, 1, VTOBJS{&a}));
	printf("isTouchingChar(&canvas, &b, VTCHAR \"░\")=%d\n", isTouchingChar(&penLayer, &b, VTCHAR "░"));
	printf("isOutside(&canvas,&b)=%d\n", isOutside(&canvas, &b));

	changeY(NULL, &b, 1);

	draw(&canvas, 3, VTOBJS{&penLayer,&a, &b});
	print(&canvas, true);
	printf("isTouching(&canvas, &b, 1, VTOBJS{&a})=%d\n", isTouching(&canvas, &b, 1, VTOBJS{&a}));
	printf("isTouchingChar(&canvas, &b, VTCHAR \"░\")=%d\n", isTouchingChar(&penLayer, &b, VTCHAR "░"));
	printf("isOutside(&canvas,&b)=%d\n", isOutside(&canvas, &b));

	clear(&penLayer);

	draw(&canvas, 3, VTOBJS{&penLayer,&a, &b});
	print(&canvas, true);
	printf("isTouching(&canvas, &b, 1, VTOBJS{&a})=%d\n", isTouching(&canvas, &b, 1, VTOBJS{&a}));
	printf("isTouchingChar(&canvas, &b, VTCHAR \"░\")=%d\n", isTouchingChar(&penLayer, &b, VTCHAR "░"));
	printf("isOutside(&canvas,&b)=%d\n", isOutside(&canvas, &b));

	changeY(NULL, &b, -4);

	draw(&canvas, 3, VTOBJS{&penLayer,&a, &b});
	print(&canvas, true);
	printf("isTouching(&canvas, &b, 1, VTOBJS{&a})=%d\n", isTouching(&canvas, &b, 1, VTOBJS{&a}));
	printf("isTouchingChar(&canvas, &b, VTCHAR \"░\")=%d\n", isTouchingChar(&penLayer, &b, VTCHAR "░"));
	printf("isOutside(&canvas,&b)=%d\n", isOutside(&canvas, &b));
	putchar('\n');
	printf("isTouching(&canvas, &b, 1, VTOBJS{&b})=%d\n", isTouching(&canvas, &b, 1, VTOBJS{&b}));

	deleteObj(&a);
	deleteObj(&b);
	deleteObj(&penLayer);
	deleteObj(&canvas);
	puts("ok");
}

int main(int argv, char **argc) {
	printf("sizeof(void*)=%lu alignof(void*)=%lu\n", sizeof(void *), alignof(void *));
	printf("sizeof(int)=%lu alignof(int)=%lu\n", sizeof(int), alignof(int));
	printf("sizeof(short)=%lu alignof(short)=%lu\n", sizeof(short), alignof(short));
	printf("sizeof(bool)=%lu alignof(bool)=%lu\n", sizeof(bool), alignof(bool));
	putchar('\n');
	printf("sizeof(CharMap)=%lu alignof(CharMap)=%lu\n", sizeof(CharMap), alignof(CharMap));
	printf("sizeof(Obj)=%lu alignof(Obj)=%lu\n", sizeof(Obj), alignof(Obj));

	putchar('\n');

	// penTest("█");
	alignTest();
	drawToStringTest();
	collisionTest();

	/*	for(unsigned int y=0; y<height;y++) {
	  for(unsigned int x = 0; x < width; x++) {
	    printf("%08" PRIx32 "(%s) ", chars[x+y*width],chars[x+y*width]);
	  }
	  putchar('\n');
	}*/

	// getchar();
}

