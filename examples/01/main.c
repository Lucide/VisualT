//
// Created by Nemo on 23/12/2019.
//

#include <visualt/visualt.h>
#include <stdio.h>
#include <inttypes.h>

typedef struct Canvas Canvas;
typedef struct Obj Obj;

int main(int argv, char **argc) {
	Canvas canvas;
	Obj textbox1, textbox2;

	about();
	initializeCanvas(&canvas, 25, 10);
	penFill(&canvas, VTCHAR "┠");
	initializeStringObj(&textbox1, VTSTR "Hello\nWorld\nfuck you");
	initializeStringObj(&textbox2, VTSTR "AAAAAAAAAAAAAA\nAAAAAAAAAAAAAA\nAAAAAAAAAAAAAA\nAAAAAAAAAAAAAA\n  \nAAAAAAAAAAAAAA");
	printf("textbox1: width %d, height %d\n", width(&textbox1), height(&textbox1));

	draw(&canvas, 1, VTOBJS{/*&textbox2,*/ &textbox1});

	setSpriteText(&textbox1, VTSTR "BOBO");

	draw(&canvas, 1, VTOBJS{/*&textbox2,*/ &textbox1});

	// for(unsigned int y=0; y<canvasHeight(&canvas);y++) {
	//   for(unsigned int x = 0; x < canvasWidth(&canvas); x++) {
	//     printf("%08" PRIx32 "(%s) ", canvas.mnaCanvas.chars[x+y*canvasWidth(&canvas)],(char*)&canvas.mnaCanvas.chars[x+y*canvasWidth(&canvas)]);
	//   }
	//   putchar('\n');
	// }
	getchar();
}