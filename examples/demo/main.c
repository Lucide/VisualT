#include "visualt/visualt.h"
#include "utilities.h"

void help(VTObj *info, int i);

int main(void) {
	FILE *const catFile = fopen("assets/cat.obj", "r");
	VTObj canvas, penLayer, info, cat, textBox;

	help(&info, 0);
	vtInitializeBlank(&canvas, 1, LTSIZES{{100, 20}}); //create a new stage of size 100x20
	vtInitializeObj(&penLayer, &canvas);
	vtInitializeFile(&cat, catFile); //load the cat obj from a file

	vtDrawAxes(&penLayer); //vtRender the x and y axes on the pen layer

	help(&info, 1);
	vtRender(&canvas, 3, LTOBJS{&penLayer, &info, &cat}); //vtRender the scene with the cat i just loaded
	vtPrint(&canvas, true);

	vtGotoXY(NULL, &cat, 35, 0); //move the cat to x:35 y:0

	help(&info, 2);
	vtRender(&canvas, 3, LTOBJS{&penLayer, &info, &cat}); //refresh the screen
	vtPrint(&canvas, true);

	vtInitializeStrings(&textBox, 1, LTSTRS{"textBox"}); //create a new textbox

	help(&info, 3);
	vtRender(&canvas, 4, LTOBJS{&penLayer, &info, &cat, &textBox});
	vtPrint(&canvas, true);

	vtSetText(&textBox, true, LTSTR "This is an example\n\nhello to everybody"); //change the text contained by textBox

	help(&info, 4);
	vtRender(&canvas, 4, LTOBJS{&penLayer, &info, &cat, &textBox});
	vtPrint(&canvas, true);

	vtGotoX(NULL, &textBox, -30); //set tbox's x position to -30

	help(&info, 5);
	vtRender(&canvas, 4, LTOBJS{&penLayer, &info, &cat, &textBox});
	vtPrint(&canvas, true);

	vtSetPenSize(&cat, 5); //set the pen size to 5 you can set a size between 1 and 6
	vtChangeX(&penLayer, &cat, -40); //change cat's x position by -40 with pen

	help(&info, 6);
	vtRender(&canvas, 4, LTOBJS{&penLayer, &info, &cat, &textBox});
	vtPrint(&canvas, true);

	vtHide(&cat); //vtHide the cat

	help(&info, 7);
	vtRender(&canvas, 4, LTOBJS{&penLayer, &info, &cat, &textBox});
	vtPrint(&canvas, true);

	vtGotoXY(NULL, &cat, vtXPosition(&textBox), vtYPosition(&textBox)); //move cat to tbox's x and y position
	vtShow(&cat); //vtShow the cat

	help(&info, 8);
	vtRender(&canvas, 4, LTOBJS{&penLayer, &info, &cat, &textBox});
	vtPrint(&canvas, true);

	vtClear(&penLayer); //empty the pen layer

	help(&info, 9);
	vtRender(&canvas, 4, LTOBJS{&penLayer, &info, &cat, &textBox});
	vtPrint(&canvas, true);

	for(int i = 0; i < 5; ++i) {
		vtStamp(&penLayer, 1, LTOBJS{&cat});
		vtChangeX(NULL, &cat, 10);
	} //a cycle where the cat stamps on the stage and moves right by 10

	help(&info, 10);
	vtRender(&canvas, 4, LTOBJS{&penLayer, &info, &cat, &textBox});
	vtPrint(&canvas, true);

	vtChangeX(NULL, &cat, 15); //move the cat again

	help(&info, 11);
	vtRender(&canvas, 4, LTOBJS{&penLayer, &info, &cat, &textBox});
	vtPrint(&canvas, true);

	vtClear(&penLayer); //vtClear the pen layer

	help(&info, 12);
	vtRender(&canvas, 4, LTOBJS{&penLayer, &info, &cat, &textBox});
	vtPrint(&canvas, true);

	vtHide(&info);
	vtChangeX(NULL, &textBox, 20);
	vtSetText(&textBox, true, LTSTR "                    Thank You!\nstay in touch for more tutorials and demo projects");
	getch();

	vtRender(&canvas, 4, LTOBJS{&penLayer, &info, &cat, &textBox});
	vtPrint(&canvas, true);

	getch();

	vtRelease(4, LTOBJS{&cat, &textBox, &penLayer, &canvas});

	help(&info, 100);

	return 0;
}

void help(VTObj *info, const int i) {
	switch(i) {
		default:
		case 0:
			vtInitializeStrings(info, 1, LTSTRS{"info"});
			puts("Press any key to start...");
			return;
		case 1:
			vtSetText(info, true, LTSTR "initializeBlank(&canvas, 1, LTSIZES{{100, 20}});\n"
																	"initializeObj(&penLayer, &canvas);\n"
																	"vtInitializeFile(&cat, \"res/cat.obj\");\n"
																	"vtDrawAxes(&penLayer);");
			break;
		case 2:
			vtSetText(info, true, LTSTR "vtGotoXY(NULL, &cat, 35, 0);");
			break;
		case 3:
			vtSetText(info, true, LTSTR "vtInitializeStrings(&textBox, 1, LTSTRS{\"textBox\"});");
			break;
		case 4:
			vtSetText(info, true, LTSTR "vtSetText(&textBox, LTSTR \"This is an example\\n\\nhello to everybody\");");
			break;
		case 5:
			vtSetText(info, true, LTSTR "vtGotoX(NULL, &textBox, -30);");
			break;
		case 6:
			vtSetText(info, true, LTSTR "vtSetPenSize(&cat, 5);\n"
																	"vtChangeX(&penLayer, &cat, -40);");
			break;
		case 7:
			vtSetText(info, true, LTSTR "vtHide(&cat);");
			break;
		case 8:
			vtSetText(info, true, LTSTR "vtGotoXY(NULL, &cat, vtXPosition(&textBox), vtYPosition(&textBox));\n"
																	"vtShow(&cat);");
			break;
		case 9:
			vtSetText(info, true, LTSTR "vtClear(&penLayer);");
			break;
		case 10:
			vtSetText(info, true, LTSTR "for(int i = 0; i < 5; ++i) {\n"
																	"  vtStamp(&penLayer, 1, LTOBJS{&cat});\n"
																	"  vtChangeX(NULL, &cat, 10);\n"
																	"}");
			break;
		case 11:
			vtSetText(info, true, LTSTR "vtChangeX(NULL, &cat, 15);");
			break;
		case 12:
			vtSetText(info, true, LTSTR "vtClear(&penLayer);");
			break;
		case 100:
			vtRelease(1, LTOBJS{info});
			return;
	}
	vtGotoXY(NULL, info, -50, 10);
	vtAlign(info, VTTOP|VTLEFT);
	getch();
}