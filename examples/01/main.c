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
  Obj textbox;

  about();
  initializeCanvas(&canvas, 25, 10);
  initializeStringObj(&textbox, (uint8_t *)"hi 👁\nhow are you?");

  draw(&canvas, 1, (Obj *[]){&textbox});

  puts("Hello World👁o");

  getchar();
}