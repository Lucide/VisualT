#include "visualt/visualt.h"
// #include <stdio.h>
#include <stdlib.h>
#include <getch.h>

typedef struct Canvas Canvas;
typedef struct Obj Obj;

void help(Obj *info, int i);

int main() {
	Obj canvas, penLayer, info, cat, textBox;

	help(&info, 0);

	initializeBlankObj(&canvas, 1, LTSIZES{{100, 20}}); //create a new stage of size 100x20
	initializeObjObj(&penLayer, &canvas);
	initializeFileObj(&cat, "obj/cat.obj"); //load the cat obj from a file
	printAxes(&penLayer); //render the x and y axes on the pen layer

	help(&info, 1);
	render(&canvas, 3, LTOBJS{&penLayer, &info, &cat}); //render the scene with the cat i just loaded
	print(&canvas, true);

	moveTo(NULL, &cat, 35, 0); //move the cat to x:35 y:0

	help(&info, 2);
	render(&canvas, 3, LTOBJS{&penLayer, &info, &cat}); //refresh the screen
	print(&canvas, true);

	initializeStringObj(&textBox, 1, LTSTRS{"textBox"}); //create a new textbox

	help(&info, 3);
	render(&canvas, 4, LTOBJS{&penLayer, &info, &cat, &textBox});
	print(&canvas, true);

	setSpriteText(&textBox, LTSTR "This is an example\n\nhello to everybody"); //change the text contained by textBox

	help(&info, 4);
	render(&canvas, 4, LTOBJS{&penLayer, &info, &cat, &textBox}); //refresh the screen
	print(&canvas, true);

	setX(NULL, &textBox, -30); //set tbox's x position to -30

	help(&info, 5);
	render(&canvas, 4, LTOBJS{&penLayer, &info, &cat, &textBox});
	print(&canvas, true);

	setPenSize(&cat, 5); //set the pen size to 5 you can set a size between 1 and 6
	changeX(&penLayer, &cat, -40); //change cat's x position by -40 with pen

	help(&info, 6);
	render(&canvas, 4, LTOBJS{&penLayer, &info, &cat, &textBox});
	print(&canvas, true);

	hide(&cat); //hide the cat

	help(&info, 7);
	render(&canvas, 4, LTOBJS{&penLayer, &info, &cat, &textBox});
	print(&canvas, true);

	moveTo(NULL, &cat, xPosition(&textBox), yPosition(&textBox)); //move cat to tbox's x and y position
	show(&cat); //show the cat

	help(&info, 8);
	render(&canvas, 4, LTOBJS{&penLayer, &info, &cat, &textBox});
	print(&canvas, true);

	clear(&penLayer); //empty the pen layer

	help(&info, 9);
	render(&canvas, 4, LTOBJS{&penLayer, &info, &cat, &textBox});
	print(&canvas, true);

	for(unsigned int i = 0; i < 5; i++) {
		stamp(&penLayer, &cat);
		changeX(NULL, &cat, 10);
	} //a cycle where the cat stamps on the stage and moves right by 10

	help(&info, 10);
	render(&canvas, 4, LTOBJS{&penLayer, &info, &cat, &textBox});
	print(&canvas, true);

	changeX(NULL, &cat, 15); //move the cat again

	help(&info, 11);
	render(&canvas, 4, LTOBJS{&penLayer, &info, &cat, &textBox});
	print(&canvas, true);

	clear(&penLayer); //clear the pen layer

	help(&info, 12);
	render(&canvas, 4, LTOBJS{&penLayer, &info, &cat, &textBox});
	print(&canvas, true);

	hide(&info);
	changeX(NULL, &textBox, 20);
	setSpriteText(&textBox, LTSTR "                    Thank You!\nstay in touch for more tutorials and demo projects");
	getch();

	render(&canvas, 4, LTOBJS{&penLayer, &info, &cat, &textBox});
	print(&canvas, true);

	getch();

	releaseObj(&cat);
	releaseObj(&textBox);
	releaseObj(&penLayer);
	releaseObj(&canvas);

	help(&info, 100);

	return 0;
}

void refresh() {
}

void help(Obj *info, const int i) {
	switch(i) {
		default:
		case 0:
			initializeStringObj(info, 1, LTSTRS{"info"});
			puts("Press any key to start...");
			return;
		case 1:
			setSpriteText(info, LTSTR "libload();\nsetstage(100,20);\ndebug_axes();\nload(\"obj/cat.obj\");");
			break;
		case 2:
			setSpriteText(info, LTSTR "move(\"cat\",35,0);");
			break;
		case 3:
			setSpriteText(info, LTSTR "text(\"tbox\");");
			break;
		case 4:
			setSpriteText(info, LTSTR "setext(\"tbox\",\"This is an example\\n\\nhello to everybody\");");
			break;
		case 5:
			setSpriteText(info, LTSTR "setx(\"tbox\",-30);");
			break;
		case 6:
			setSpriteText(info, LTSTR "pendown(\"cat\");\npensize(5);\nchx(\"cat\",-40);");
			break;
		case 7:
			setSpriteText(info, LTSTR "hide(\"cat\");");
			break;
		case 8:
			setSpriteText(info, LTSTR "penup(\"cat\");\nmove(\"cat\",gox(\"tbox\"),goy(\"tbox\"));\nshow(\"cat\");");
			break;
		case 9:
			setSpriteText(info, LTSTR "penclear();");
			break;
		case 10:
			setSpriteText(info, LTSTR "for(i=0;i<5;i=i+1){\n stamp(\"cat\");\n chx(\"cat\",10);\n}");
			break;
		case 11:
			setSpriteText(info, LTSTR "chx(\"cat\",15);");
			break;
		case 12:
			setSpriteText(info, LTSTR "penclear();");
			break;
		case 100:
			releaseObj(info);
			return;
	}
	moveTo(NULL, info, -50, 10);
	align(info, 0);
	getch();
}