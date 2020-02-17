#include "visualt/visualtUnprefixed.h"
#include "getch.h"

typedef struct vtObj Obj;

void help(Obj *info, int i);

int main() {
	Obj canvas, penLayer, info, cat, textBox;

	help(&info, 0);

	initializeBlank(&canvas, 1, LTSIZES{{100, 20}}); //create a new stage of size 100x20
	initializeObj(&penLayer, &canvas);
	initializeFile(&cat, "res/cat.obj"); //load the cat obj from a file
	printAxes(&penLayer); //render the x and y axes on the pen layer

	help(&info, 1);
	render(&canvas, 3, LTOBJS{&penLayer, &info, &cat}); //render the scene with the cat i just loaded
	print(&canvas, true);

	gotoXY(NULL, &cat, 35, 0); //move the cat to x:35 y:0

	help(&info, 2);
	render(&canvas, 3, LTOBJS{&penLayer, &info, &cat}); //refresh the screen
	print(&canvas, true);

	initializeString(&textBox, 1, LTSTRS{"textBox"}); //create a new textbox

	help(&info, 3);
	render(&canvas, 4, LTOBJS{&penLayer, &info, &cat, &textBox});
	print(&canvas, true);

	setText(&textBox, LTSTR "This is an example\n\nhello to everybody"); //change the text contained by textBox

	help(&info, 4);
	render(&canvas, 4, LTOBJS{&penLayer, &info, &cat, &textBox});
	print(&canvas, true);

	gotoX(NULL, &textBox, -30); //set tbox's x position to -30

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

	gotoXY(NULL, &cat, xPosition(&textBox), yPosition(&textBox)); //move cat to tbox's x and y position
	show(&cat); //show the cat

	help(&info, 8);
	render(&canvas, 4, LTOBJS{&penLayer, &info, &cat, &textBox});
	print(&canvas, true);

	clear(&penLayer); //empty the pen layer

	help(&info, 9);
	render(&canvas, 4, LTOBJS{&penLayer, &info, &cat, &textBox});
	print(&canvas, true);

	for(unsigned int i = 0; i < 5; i++) {
		stamp(&penLayer, 1, LTOBJS{&cat});
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
	setText(&textBox, LTSTR "                    Thank You!\nstay in touch for more tutorials and demo projects");
	getch();

	render(&canvas, 4, LTOBJS{&penLayer, &info, &cat, &textBox});
	print(&canvas, true);

	getch();

	release(4, LTOBJS{&cat, &textBox, &penLayer, &canvas});

	help(&info, 100);

	return 0;
}

void refresh() {
}

void help(Obj *info, const int i) {
	switch(i) {
		default:
		case 0:
			initializeString(info, 1, LTSTRS{"info"});
			puts("Press any key to start...");
			return;
		case 1:
			setText(info, LTSTR "initializeBlank(&canvas, 1, LTSIZES{{100, 20}});\n"
													"initializeObj(&penLayer, &canvas);\n"
													"initializeFile(&cat, \"res/cat.obj\");\n"
													"printAxes(&penLayer);");
			break;
		case 2:
			setText(info, LTSTR "gotoXY(NULL, &cat, 35, 0);");
			break;
		case 3:
			setText(info, LTSTR "initializeString(&textBox, 1, LTSTRS{\"textBox\"});");
			break;
		case 4:
			setText(info, LTSTR "setText(&textBox, LTSTR \"This is an example\\n\\nhello to everybody\");");
			break;
		case 5:
			setText(info, LTSTR "gotoX(NULL, &textBox, -30);");
			break;
		case 6:
			setText(info, LTSTR "setPenSize(&cat, 5);\n"
													"changeX(&penLayer, &cat, -40);");
			break;
		case 7:
			setText(info, LTSTR "hide(&cat);");
			break;
		case 8:
			setText(info, LTSTR "gotoXY(NULL, &cat, xPosition(&textBox), yPosition(&textBox));\n"
													"show(&cat);");
			break;
		case 9:
			setText(info, LTSTR "clear(&penLayer);");
			break;
		case 10:
			setText(info, LTSTR "for(unsigned int i = 0; i < 5; i++) {\n"
													"  stamp(&penLayer, 1, LTOBJS{&cat});\n"
													"  changeX(NULL, &cat, 10);\n"
													"}");
			break;
		case 11:
			setText(info, LTSTR "changeX(NULL, &cat, 15);");
			break;
		case 12:
			setText(info, LTSTR "clear(&penLayer);");
			break;
		case 100:
			release(1,LTOBJS{info});
			return;
	}
	gotoXY(NULL, info, -50, 10);
	align(info, 0);
	getch();
}