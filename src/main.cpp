#include "pilates.h"

#include "debug.h"

#include <stdio.h>

int main() {
  Node div = divNode();
  div.width = 24;
  div.height = 14;

  setFlexDirection(&div, PILATES_ROW);
  setJustifyContent(&div, PILATES_SPACE_AROUND);
  //setAlignItems(&div, PILATES_ALIGN_CENTER);

  Node div1 = divNode();
  div1.width = 4;
  div1.height = 4;
  div1.id = 1;

  Node div2 = divNode();
  div2.width = 4;
  div2.height = 4;
  div2.id = 2;

  Node div3 = divNode();
  div3.width = 4;
  div3.height = 4;
  div3.id = 3;

  Node children[3] = {div1, div2, div3};
  div.children = children;
  div.num_children = 3;

  layoutNodes(&div, asciiMeasureText);

  printNode(&div, printf);

  printf("\n");

  asciiRender(&div);

  return 0;
}
