//Cavasin Riccardo

#include<stdio.h>
#include"../../VisualT/visualt.h"

void help(int i);

int main(int argv, char**argc){
	libload();
	//required! this initializes
	//the visualt libray

	int i;
	text("help");
	//ignore this

	setstage(100,20);
	//here i create a new stage
	//of size 100x20

	debug_axes();
	//here i draw the x and
	//y axes on the pen layer

	load("obj/cat.obj");
	//here i load the cat obj
	//from a file

		getch(0);
		help(1);
		//ignore this

	draw();
	//here i render the scene
	//with the cat i just loaded

	move("cat",35,0);
	//here i move the cat to
	//x 35 y 0

		getch(0);
		help(2);
		//ignore this

	draw();
	//i render again to refresh
	//the screen

	text("tbox");
	//here i create a new textbox
	//remember that the name can't
	//be longer than 5 characters

		getch(0);
		help(3);
		//ignore this

	draw();
	//i refresh the screen

	setext("tbox","This is an example\n\nhello to everybody");
	//here i change the text
	//contained by tbox

		getch(0);
		help(4);
		//ignore this

	draw();
	//i refresh the screen

	setx("tbox",-30);
	//here i set tbox's x
	//position to -30

		getch(0);
		help(5);
		//ignore this

	draw();
	//i refresh the screen

	pendown("cat");
	//here i enable cat's pen
	//by default the size is set to 1
	//and the character  to '#'

	pensize(5);
	//here i set the pen size to 3
	//you can set a size between 1 and 6

	chx("cat",-40);
	//here i change cat's x
	//position by -40

		getch(0);
		help(6);
		//ignore this

	draw();
	//i refresh the screen

	hide("cat");
	//here i hide the cat

		getch(0);
		help(7);
		//ignore this

	draw();
	//i refresh the screen

	penup("cat");
	//here i disable cat's pen

	move("cat",gox("tbox"),goy("tbox"));
	//here i move cat to tbox's
	//x and y position

	show("cat");
	//here i show the cat

		getch(0);
		help(8);
		//ignore this

	draw();
	//i refresh the screen

	penclear();
	//here i empty the pen layer

		getch(0);
		help(9);
		//ignore this

	draw();
	//i refresh the screen

	for(i=0;i<5;i=i+1){
		stamp("cat");
		chx("cat",10);
	}
	//here i made a cycle where
	//the cat stamps on the
	//stage and moves right
	//by 10

		getch(0);
		help(10);
		//ignore this

	draw();
	//i refresh the screen

	chx("cat",15);
	//here i move the cat
	//again

		getch(0);
		help(11);
		//ignore this

	draw();
	//i refresh the screen

	penclear();
	//here i empty the pen layer

		getch(0);
		help(12);
		//ignore this

	draw();
	//i refresh the screen

	hide("help");
	chx("tbox",20);
	setext("tbox","\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\vThank You!\nstay in touch for more tutorials and demo projects");
	getch(0);
	draw();
	//you know this by now

	getch(0);
	debug_global();
	debug_obj("cat");
	debug_obj("tbox");
	//what i have now
return 0;
}

void help(int i){
	switch(i){
		case 1:
			setext("help","libload();\nsetstage(100,20);\ndebug_axes();\nload(\"obj/cat.obj\");");
		break;
		case 2:
			setext("help","move(\"cat\",35,0);");
		break;
		case 3:
			setext("help","text(\"tbox\");");
		break;
		case 4:
			setext("help","setext(\"tbox\",\"This is an example\\n\\nhello to everybody\");");
		break;
		case 5:
			setext("help","setx(\"tbox\",-30);");
		break;
		case 6:
			setext("help","pendown(\"cat\");\npensize(5);\nchx(\"cat\",-40);");
		break;
		case 7:
			setext("help","hide(\"cat\");");
		break;
		case 8:
			setext("help","penup(\"cat\");\nmove(\"cat\",gox(\"tbox\"),goy(\"tbox\"));\nshow(\"cat\");");
		break;
		case 9:
			setext("help","penclear();");
		break;
		case 10:
			setext("help","for(i=0;i<5;i=i+1){\n stamp(\"cat\");\n chx(\"cat\",10);\n}");
		break;
		case 11:
			setext("help","chx(\"cat\",15);");
		break;
		case 12:
			setext("help","penclear();");
		break;
	}
	move("help",-50,10);
	textalize("help",0);
}
