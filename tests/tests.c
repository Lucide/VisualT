#include "visualt/visualt.h"
#include "visualt/import-xp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef VTObj Obj;

void penTest(VTChar penChar) {
	puts("pen stroke test:");
	Obj canvas, p;
	vtInitializeBlank(&canvas, 1, LTSIZES{{100, 20}});
	vtInitializeStrings(&p, 1, LTSTRS{"p"});
	vtSetPenGlyph(&p, penChar);

	vtGotoXY(NULL, &p, -57, 7);
	for(unsigned int penSize = 1; penSize <= 6; ++penSize) {
		vtChangeX(NULL, &p, 13);
		vtSetPenSize(&p, penSize);
		vtChangeX(&canvas, &p, 0);
	}

	vtGotoXY(NULL, &p, -46, -2);
	for(unsigned int i = 1; i <= 6; ++i) {
		vtSetPenSize(&p, i);
		vtChangeX(&canvas, &p, 10);
	}

	vtGotoXY(NULL, &p, 40, 6);
	for(unsigned int i = 1; i <= 6; ++i) {
		vtSetPenSize(&p, i);
		vtChangeY(&canvas, &p, -2);
	}

	vtPrint(&canvas, true);
	vtRelease(2, LTOBJS{&canvas, &p});
	puts("done");
}

void alignTest(void) {
	puts("align test:");
	Obj canvas, target, viewfinder;
	vtInitializeBlank(&canvas, 1, LTSIZES{{50, 10}});
	vtInitializeStrings(&target, 1, LTSTRS{"0"});
	vtInitializeStrings(&viewfinder, 1, LTSTRS{"██\n██"});

	// top left 2x2
	vtGotoXY(NULL, &viewfinder, -23, 3);
	vtGotoXY(NULL, &target, -23, 3);
	vtAlign(&viewfinder, VT_TOP|VT_LEFT);
	vtStamp(&canvas, 2, LTOBJS{&viewfinder, &target});

	// top right 2x2
	vtGotoXY(NULL, &viewfinder, -18, 3);
	vtGotoXY(NULL, &target, -18, 3);
	vtAlign(&viewfinder, VT_TOP|VT_RIGHT);
	vtStamp(&canvas, 2, LTOBJS{&viewfinder, &target});

	// bottom left 2x2
	vtGotoXY(NULL, &viewfinder, -13, 3);
	vtGotoXY(NULL, &target, -13, 3);
	vtAlign(&viewfinder, VT_BOTTOM|VT_LEFT);
	vtStamp(&canvas, 2, LTOBJS{&viewfinder, &target});

	// bottom right 2x2
	vtGotoXY(NULL, &viewfinder, -7, 3);
	vtGotoXY(NULL, &target, -7, 3);
	vtAlign(&viewfinder, VT_BOTTOM|VT_RIGHT);
	vtStamp(&canvas, 2, LTOBJS{&viewfinder, &target});

	vtSetText(&viewfinder, true, LTSTR "███\n███");

	// top left 3x2
	vtGotoXY(NULL, &viewfinder, -2, 3);
	vtGotoXY(NULL, &target, -2, 3);
	vtAlign(&viewfinder, VT_TOP|VT_LEFT);
	vtStamp(&canvas, 2, LTOBJS{&viewfinder, &target});

	// top right 3x2
	vtGotoXY(NULL, &viewfinder, 4, 3);
	vtGotoXY(NULL, &target, 4, 3);
	vtAlign(&viewfinder, VT_TOP|VT_RIGHT);
	vtStamp(&canvas, 2, LTOBJS{&viewfinder, &target});

	// bottom left 3x2
	vtGotoXY(NULL, &viewfinder, 10, 3);
	vtGotoXY(NULL, &target, 10, 3);
	vtAlign(&viewfinder, VT_BOTTOM|VT_LEFT);
	vtStamp(&canvas, 2, LTOBJS{&viewfinder, &target});

	// bottom right 3x2
	vtGotoXY(NULL, &viewfinder, 16, 3);
	vtGotoXY(NULL, &target, 16, 3);
	vtAlign(&viewfinder, VT_BOTTOM|VT_RIGHT);
	vtStamp(&canvas, 2, LTOBJS{&viewfinder, &target});

	vtSetText(&viewfinder, true, LTSTR "██\n██\n██");

	// top left 2x3
	vtGotoXY(NULL, &viewfinder, -23, -1);
	vtGotoXY(NULL, &target, -23, -1);
	vtAlign(&viewfinder, VT_TOP|VT_LEFT);
	vtStamp(&canvas, 2, LTOBJS{&viewfinder, &target});

	// top right 2x3
	vtGotoXY(NULL, &viewfinder, -18, -1);
	vtGotoXY(NULL, &target, -18, -1);
	vtAlign(&viewfinder, VT_TOP|VT_RIGHT);
	vtStamp(&canvas, 2, LTOBJS{&viewfinder, &target});

	// bottom left 2x3
	vtGotoXY(NULL, &viewfinder, -13, -1);
	vtGotoXY(NULL, &target, -13, -1);
	vtAlign(&viewfinder, VT_BOTTOM|VT_LEFT);
	vtStamp(&canvas, 2, LTOBJS{&viewfinder, &target});

	// bottom right 2x3
	vtGotoXY(NULL, &viewfinder, -7, -1);
	vtGotoXY(NULL, &target, -7, -1);
	vtAlign(&viewfinder, VT_BOTTOM|VT_RIGHT);
	vtStamp(&canvas, 2, LTOBJS{&viewfinder, &target});

	vtSetText(&viewfinder, true, LTSTR "███\n███\n███");

	// top left 3x3
	vtGotoXY(NULL, &viewfinder, -2, -1);
	vtGotoXY(NULL, &target, -2, -1);
	vtAlign(&viewfinder, VT_TOP|VT_LEFT);
	vtStamp(&canvas, 2, LTOBJS{&viewfinder, &target});

	// top right 3x3
	vtGotoXY(NULL, &viewfinder, 4, -1);
	vtGotoXY(NULL, &target, 4, -1);
	vtAlign(&viewfinder, VT_TOP|VT_RIGHT);
	vtStamp(&canvas, 2, LTOBJS{&viewfinder, &target});

	// bottom left 3x3
	vtGotoXY(NULL, &viewfinder, 10, -1);
	vtGotoXY(NULL, &target, 10, -1);
	vtAlign(&viewfinder, VT_BOTTOM|VT_LEFT);
	vtStamp(&canvas, 2, LTOBJS{&viewfinder, &target});

	// bottom right 3x3
	vtGotoXY(NULL, &viewfinder, 16, -1);
	vtGotoXY(NULL, &target, 16, -1);
	vtAlign(&viewfinder, VT_BOTTOM|VT_RIGHT);
	vtStamp(&canvas, 2, LTOBJS{&viewfinder, &target});

	vtPrint(&canvas, true);

	vtRelease(3, LTOBJS{&viewfinder, &target, &canvas});
	puts("done");
}

void drawToStringTest(void) {
	puts("draw to string test:");
	Obj canvas;
	unsigned int length;
	unsigned char *s = NULL;
	vtInitializeStrings(&canvas, 1, LTSTRS{"𝄞𝄞𝄞\n𝄞𝄞𝄞"});
	length = vtPrintToString(&canvas, true, &s);
	printf("length: %d\n%s\n", length, s);
	free(s);
	s = NULL;
	length = vtPrintToString(&canvas, false, &s);
	printf("length: %d\n%s\n", length, s);
	free(s);
	s = NULL;

	vtRelease(1, LTOBJS{&canvas});
	puts("done");
}

void collisionTest(void) {
	puts("collision test:");
	Obj canvas, penLayer, pot, circle;

	vtInitializeBlank(&canvas, 1, LTSIZES{{60, 16}});
	vtInitializeObj(&penLayer, &canvas);
	vtInitializeStrings(&pot, 1, LTSTRS{"|░░░░|\n\\░░░░/"});
	vtInitializeStrings(&circle, 1, LTSTRS{"\v\v\v____\n\v.'\v\v\v\v`.\n/\v\v\v\v\v\v\v\v\\\n|\v\v\v\v\v\v\v\v|\n\\\v\v\v\v\v\v\v\v/\n\v`.____.'"});

	vtSetPenSize(&pot, 3);
	vtSetPenGlyph(&pot, vtChar("░"));
	vtGotoXY(NULL, &pot, 18, 6);
	vtGotoXY(&penLayer, &pot, 18, -6);

	vtRender(&canvas, 3, LTOBJS{&penLayer, &pot, &circle});
	vtPrint(&canvas, true);
	printf("vtIsTouching(&canvas, &circle, 1, VTOBJS{&pot})=%d\n", vtIsTouching(&canvas, &circle, 1, LTOBJS{&pot}));
	printf("vtIsTouchingGlyph(&penLayer, &circle, vtChar(\"░\"))=%d\n", vtIsTouchingGlyph(&penLayer, &circle, vtChar("░")));
	printf("vtIsOutside(&canvas, &circle)=%d\n", vtIsOutside(&canvas, &circle));

	vtGotoX(NULL, &circle, 18);

	vtRender(&canvas, 3, LTOBJS{&penLayer, &pot, &circle});
	vtPrint(&canvas, true);
	printf("vtIsTouching(&canvas, &circle, 1, VTOBJS{&pot})=%d\n", vtIsTouching(&canvas, &circle, 1, LTOBJS{&pot}));
	printf("vtIsTouchingGlyph(&penLayer, &circle, vtChar(\"░\"))=%d\n", vtIsTouchingGlyph(&penLayer, &circle, vtChar("░")));
	printf("vtIsOutside(&canvas, &circle)=%d\n", vtIsOutside(&canvas, &circle));

	vtGotoY(NULL, &circle, -5);

	vtRender(&canvas, 3, LTOBJS{&penLayer, &pot, &circle});
	vtPrint(&canvas, true);
	printf("vtIsTouching(&canvas, &circle, 1, VTOBJS{&pot})=%d\n", vtIsTouching(&canvas, &circle, 1, LTOBJS{&pot}));
	printf("vtIsTouchingGlyph(&penLayer, &circle, vtChar(\"░\"))=%d\n", vtIsTouchingGlyph(&penLayer, &circle, vtChar("░")));
	printf("vtIsOutside(&canvas, &circle)=%d\n", vtIsOutside(&canvas, &circle));

	vtChangeY(NULL, &circle, 1);

	vtRender(&canvas, 3, LTOBJS{&penLayer, &pot, &circle});
	vtPrint(&canvas, true);
	printf("vtIsTouching(&canvas, &circle, 1, VTOBJS{&pot})=%d\n", vtIsTouching(&canvas, &circle, 1, LTOBJS{&pot}));
	printf("vtIsTouchingGlyph(&penLayer, &circle, vtChar(\"░\"))=%d\n", vtIsTouchingGlyph(&penLayer, &circle, vtChar("░")));
	printf("vtIsOutside(&canvas, &circle)=%d\n", vtIsOutside(&canvas, &circle));

	vtClear(&penLayer);

	vtRender(&canvas, 3, LTOBJS{&penLayer, &pot, &circle});
	vtPrint(&canvas, true);
	printf("vtIsTouching(&canvas, &circle, 1, VTOBJS{&pot})=%d\n", vtIsTouching(&canvas, &circle, 1, LTOBJS{&pot}));
	printf("vtIsTouchingGlyph(&penLayer, &circle, vtChar(\"░\"))=%d\n", vtIsTouchingGlyph(&penLayer, &circle, vtChar("░")));
	printf("vtIsOutside(&canvas, &circle)=%d\n", vtIsOutside(&canvas, &circle));

	vtChangeY(NULL, &circle, -4);

	vtRender(&canvas, 3, LTOBJS{&penLayer, &pot, &circle});
	vtPrint(&canvas, true);
	printf("vtIsTouching(&canvas, &circle, 1, VTOBJS{&pot})=%d\n", vtIsTouching(&canvas, &circle, 1, LTOBJS{&pot}));
	printf("vtIsTouchingGlyph(&penLayer, &circle, vtChar(\"░\"))=%d\n", vtIsTouchingGlyph(&penLayer, &circle, vtChar("░")));
	printf("vtIsOutside(&canvas, &circle)=%d\n", vtIsOutside(&canvas, &circle));
	putchar('\n');
	printf("vtIsTouching(&canvas, &circle, 1, VTOBJS{&circle})=%d\n", vtIsTouching(&canvas, &circle, 1, LTOBJS{&circle}));

	vtRelease(4, LTOBJS{&pot, &circle, &penLayer, &canvas});
	puts("done");
}

void cloneResizeTest(void) {
	puts("clone resize test:");
	Obj objA, objB;
	vtInitializeStrings(&objA, 2, LTSTRS{"𝄞𝄞𝄞\n𝄞𝄞𝄞", "𝄢𝄢𝄢𝄢\n𝄢𝄢𝄢𝄢"});

	vtInitializeObj(&objB, &objA);
	vtPrint(&objB, true);
	vtSetSprite(&objB, 1);
	vtPrint(&objB, true);

	vtCloneSprite(&objA, 0, &objB, 1);
	vtPrint(&objA, true);

	vtResize(&objA, 1, 1);
	vtPrint(&objA, true);

	vtRelease(2, LTOBJS{&objA, &objB});
	puts("done");
}

void vtSetSpriteTextTest(void) {
	puts("vtSetSpriteText test:");
	Obj a;
	vtInitializeStrings(&a, 1, LTSTRS{"𝄢𝄢𝄢𝄢\n𝄢𝄢𝄢𝄢"});
	vtPrint(&a, true);
	vtSetText(&a, true, LTSTR "𝄞𝄞𝄞\n𝄞𝄞𝄞");
	vtPrint(&a, true);

	vtRelease(1, LTOBJS{&a});
	puts("done");
}

void dynamicMemoryTest(void) {
	puts("dynamic memory test:");

	puts("1-vtInitializeBlank() with dynamic array of pointer to (dynamic) unsigned int[2]");
	{
		Obj a;
		unsigned int (*v)[2] = malloc(2*sizeof(unsigned int (*)[2]));
		v[0][0] = 1;
		v[0][1] = 2;
		v[1][0] = 2;
		v[1][1] = 1;
		vtInitializeBlank(&a, 2, (VTSizes)v);
		free(v);
		vtPrint(&a, true);
		vtNextSprite(&a);
		vtPrint(&a, true);
		vtRelease(1, LTOBJS{&a});
	}

	puts("2-vtInitializeArray() with dynamic array of unsigned int");
	{
		Obj a;
		VTChar *v = malloc(4*sizeof(VTChar));
		v[0] = 1;
		v[1] = 1;
		v[2] = 1;
		v[3] = 0;
		vtInitializeArray(&a, v);
		free(v);
		vtPrint(&a, true);
		vtRelease(1, LTOBJS{&a});
	}

	puts("3-vtInitializeStrings() with dynamic array of pointer to (dynamic) char");
	{
		Obj a;
		char **strings = malloc(2*sizeof(uint8_t *));
		strings[0] = malloc(6*sizeof(char));
		strcpy((char *)strings[0], "𝄞");
		strings[1] = malloc(7*sizeof(char));
		strcpy((char *)strings[1], "𝄢");
		vtInitializeStrings(&a, 2, (VTStrs)strings);
		free(strings[0]);
		free(strings[1]);
		free(strings);
		vtPrint(&a, true);
		vtNextSprite(&a);
		vtPrint(&a, true);
		vtRelease(1, LTOBJS{&a});
	}

	puts("4-vtFill() with dynamic array of char");
	{
		char *c = malloc(5*sizeof(char));
		strcpy(c, "𝄞");
		Obj a;
		vtInitializeBlank(&a, 1, LTSIZES{{2, 2}});
		vtFill(&a, vtChar(c));
		free(c);
		vtPrint(&a, true);
		vtRelease(1, LTOBJS{&a});
	}

	puts("5-vtInitializeStrings(), setY(), vtRender() with dynamic array of pointer to (dynamic) Obj");
	{
		Obj canvas;
		vtInitializeBlank(&canvas, 1, LTSIZES{{3, 3}});
		Obj **objs = malloc(2*sizeof(Obj *));
		objs[0] = malloc(sizeof(Obj));
		vtInitializeStrings(objs[0], 1, LTSTRS{"𝄞"});
		vtGotoY(NULL, objs[0], 1);
		objs[1] = malloc(sizeof(Obj));
		vtInitializeStrings(objs[1], 1, LTSTRS{"𝄢"});
		vtGotoY(NULL, objs[1], -1);

		vtRender(&canvas, 2, (VTObjs)objs);
		vtPrint(&canvas, true);

		vtRelease(3, LTOBJS{&canvas, objs[0], objs[1]});
		free(objs[0]);
		free(objs[1]);
		free(objs);
	}

	puts("done");
}

void printAxesTest(void) {
	puts("vtDrawAxes() test:");
	Obj canvas;
	vtInitializeBlank(&canvas, 1, LTSIZES{{9, 5}});
	vtDrawAxes(&canvas);
	vtPrint(&canvas, true);
	vtResize(&canvas, 8, 6);
	vtDrawAxes(&canvas);
	vtPrint(&canvas, true);
	vtRelease(1, LTOBJS{&canvas});
	puts("done");
}

void serializationTest(void) {
	puts("serialization test:");
	Obj obj;
	vtInitializeStrings(&obj, 1, LTSTRS{"a ¶ ╭ 𝄞"});
	uint32_t *const v = malloc(vtSerializedArraySize(&obj));
	vtSerialize(&obj, v);
	vtSerialize(&obj, NULL);
	putchar('\n');
	vtRelease(1, LTOBJS{&obj});
	vtInitializeArray(&obj, v);
	vtPrint(&obj, true);
	free(v);
	vtRelease(1, LTOBJS{&obj});
	puts("done");
}

void xpImportTest(void) {
	#ifdef VISUALT_IMPORT_XP
	puts("xp import test:");

	puts("1-VT_XP_LAYERS");
	{
		VTObj obj;
		FILE *const file = fopen("assets/object-sprite.xp", "rb");
		FILE *const map = fopen("assets/utf8.txt", "r");
		vtInitializeXp(&obj, VT_XP_LAYERS, map, &file, 1);
		for(unsigned int i = 0; i < vtSprites(&obj); ++i) {
			vtPrint(&obj, true);
			vtNextSprite(&obj);
		}
		vtRelease(1, LTOBJS{&obj});
	}
	{
		VTObj obj;
		FILE *const file1 = fopen("assets/object-sprite.xp", "rb");
		FILE *const file2 = fopen("assets/VCat.xp", "rb");
		FILE *const map = fopen("assets/utf8.txt", "r");
		vtInitializeXp(&obj, VT_XP_FILES, map, (FILE *[]){file1, file2}, 2);
		for(unsigned int i = 0; i < vtSprites(&obj); ++i) {
			vtPrint(&obj, true);
			vtNextSprite(&obj);
		}
		vtRelease(1, LTOBJS{&obj});
	}
	#else
	puts("xp importer is disabled:");
	#endif
	puts("done");
}

void dinosaur(void) {
	puts("dinosaur:");
	Obj dinosaur;
	vtInitializeStrings(&dinosaur, 1, LTSTRS{"\v\v\v\v\v\v\v\v\v\v\v\v╭━━━\n\v\v\v╭━━╮\v\v\v\v\v┃RAWR\n\v\v╭╯┊◣╰━━━╮\v╰┳━━\n\v\v┃┊┊┊╱▽▽▽┛\v\v┃\v\v\n\v\v┃┊┊┊▏━━━━━━╯\v\v\n━━╯┊┊┊╲△△△┓\v\v\v\v\v\n┊┊┊┊╭━━━━━╯\v\v\v\v\v"});
	vtPrint(&dinosaur, false);
	vtRelease(1, LTOBJS{&dinosaur});
	puts("done");
}

int main(void) {
	vtAbout();
	puts("\nThese tests are meant to be ran through a memcheck: https://valgrind.org/\n");

	//	printf("sizeof(void*)=%lu alignof(void*)=%lu\n", sizeof(void *), alignof(void *));
	//	printf("sizeof(int)=%lu alignof(int)=%lu\n", sizeof(int), alignof(int));
	//	printf("sizeof(short)=%lu alignof(short)=%lu\n", sizeof(short), alignof(short));
	//	printf("sizeof(bool)=%lu alignof(bool)=%lu\n", sizeof(bool), alignof(bool));
	//	putchar('\n');
	//	printf("sizeof(CharMap)=%lu alignof(CharMap)=%lu\n", sizeof(CharMap), alignof(CharMap));
	//	printf("sizeof(Obj)=%lu alignof(Obj)=%lu\n", sizeof(Obj), alignof(Obj));

	penTest(vtChar("█"));
	alignTest();
	drawToStringTest();
	collisionTest();
	cloneResizeTest();
	vtSetSpriteTextTest();
	dynamicMemoryTest();
	printAxesTest();
	serializationTest();
	xpImportTest();
	// dinosaur();



	// getchar();
}

