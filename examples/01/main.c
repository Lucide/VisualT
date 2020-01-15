#include <visualt/visualt.h>
// #include <stdio.h>
#include <stdlib.h>
#include <getch.h>

typedef struct Canvas Canvas;
typedef struct Obj Obj;

void help(Obj *info, int i);

int main(int argc, char **argv) {
	Canvas canvas;
	Obj info, cat, textBox;

	help(&info, 0);

	initializeCanvas(&canvas, 100, 20); //create a new stage of size 100x20
	initializeFileObj(&cat, "obj/cat.obj"); //load the cat obj from a file
	printAxes(&canvas); //draw the x and y axes on the pen layer

	help(&info, 1);
	draw(&canvas, 2, VTOBJS{&info, &cat}); //render the scene with the cat i just loaded

	moveTo(NULL, &cat, 35, 0); //move the cat to x:35 y:0

	help(&info, 2);
	draw(&canvas, 2, VTOBJS{&info, &cat}); //refresh the screen

	initializeStringObj(&textBox, 1, VTSTRS{"textBox"}); //create a new textbox

	help(&info, 3);
	draw(&canvas, 3, VTOBJS{&info, &cat, &textBox});

	setSpriteText(&textBox, VTSTR "This is an example\n\nhello to everybody"); //change the text contained by textBox

	help(&info, 4);
	draw(&canvas, 3, VTOBJS{&info, &cat, &textBox}); //refresh the screen

	setX(NULL, &textBox, -30); //set tbox's x position to -30

	help(&info, 5);
	draw(&canvas, 3, VTOBJS{&info, &cat, &textBox});

	setPenSize(&cat, 5); //set the pen size to 5 you can set a size between 1 and 6
	changeX(&canvas, &cat, -40); //change cat's x position by -40 with pen

	help(&info, 6);
	draw(&canvas, 3, VTOBJS{&info, &cat, &textBox});

	hide(&cat); //hide the cat

	help(&info, 7);
	draw(&canvas, 3, VTOBJS{&info, &cat, &textBox});

	moveTo(NULL, &cat, xPosition(&textBox), yPosition(&textBox)); //move cat to tbox's x and y position
	show(&cat); //show the cat

	help(&info, 8);
	draw(&canvas, 3, VTOBJS{&info, &cat, &textBox});

	clear(&canvas); //empty the pen layer

	help(&info, 9);
	draw(&canvas, 3, VTOBJS{&info, &cat, &textBox});

	for(unsigned int i = 0; i < 5; i++) {
		stamp(&canvas, &cat);
		changeX(NULL, &cat, 10);
	} //a cycle where the cat stamps on the stage and moves right by 10

	help(&info, 10);
	draw(&canvas, 3, VTOBJS{&info, &cat, &textBox});

	changeX(NULL, &cat, 15); //move the cat again

	help(&info, 11);
	draw(&canvas, 3, VTOBJS{&info, &cat, &textBox});

	clear(&canvas); //clear the pen layer

	help(&info, 12);
	draw(&canvas, 3, VTOBJS{&info, &cat, &textBox});

	hide(&info);
	changeX(NULL, &textBox, 20);
	setSpriteText(&textBox, VTSTR "                    Thank You!\nstay in touch for more tutorials and demo projects");
	getch();

	draw(&canvas, 3, VTOBJS{&info, &cat, &textBox});

	getch();

	deleteObj(&cat);
	deleteObj(&textBox);
	deleteCanvas(&canvas);

	help(&info, 100);

	return 0;
}

void help(Obj *info, const int i) {
	switch(i) {
		default:
		case 0:
			initializeStringObj(info, 1, VTSTRS{"info"});
			puts("Press any key to start...");
			return;
		case 1:
			setSpriteText(info, VTSTR "libload();\nsetstage(100,20);\ndebug_axes();\nload(\"obj/cat.obj\");");
			break;
		case 2:
			setSpriteText(info, VTSTR "move(\"cat\",35,0);");
			break;
		case 3:
			setSpriteText(info, VTSTR "text(\"tbox\");");
			break;
		case 4:
			setSpriteText(info, VTSTR "setext(\"tbox\",\"This is an example\\n\\nhello to everybody\");");
			break;
		case 5:
			setSpriteText(info, VTSTR "setx(\"tbox\",-30);");
			break;
		case 6:
			setSpriteText(info, VTSTR "pendown(\"cat\");\npensize(5);\nchx(\"cat\",-40);");
			break;
		case 7:
			setSpriteText(info, VTSTR "hide(\"cat\");");
			break;
		case 8:
			setSpriteText(info, VTSTR "penup(\"cat\");\nmove(\"cat\",gox(\"tbox\"),goy(\"tbox\"));\nshow(\"cat\");");
			break;
		case 9:
			setSpriteText(info, VTSTR "penclear();");
			break;
		case 10:
			setSpriteText(info, VTSTR "for(i=0;i<5;i=i+1){\n stamp(\"cat\");\n chx(\"cat\",10);\n}");
			break;
		case 11:
			setSpriteText(info, VTSTR "chx(\"cat\",15);");
			break;
		case 12:
			setSpriteText(info, VTSTR "penclear();");
			break;
		case 100:
			deleteObj(info);
			return;
	}
	moveTo(NULL, info, -50, 10);
	align(info, 0);
	getch();
}