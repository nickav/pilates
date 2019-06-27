#include "pilates.h"

#include "debug.h"

#include <stdio.h>

int main() {
  /*
  Node div = divNode();
  div.width = 24;
  div.height = 32;

  setFlexDirection(&div, PILATES_ROW);
  setJustifyContent(&div, PILATES_SPACE_BETWEEN);
  setAlignItems(&div, PILATES_ALIGN_CENTER);

  Node div1 = divNode();
  setNodeSize(&div1, 4, 4);
  div1.id = 1;

  Node div2 = divNode();
  setNodeSize(&div2, 4, 4);
  div2.id = 2;

  Node div3 = divNode();
  setNodeSize(&div3, 4, 4);
  div3.id = 3;

  Node div4 = divNode();
  setNodeSize(&div4, 4, 4);
  div4.id = 4;

  Node children[4] = {div1, div2, div3, div4};
  div.children = children;
  div.num_children = 4;
  */

  Node div = divNode();
  div.width = 8;
  div.height = 16;

  setFlexDirection(&div, PILATES_COLUMN);
  setJustifyContent(&div, PILATES_ALIGN_CENTER);
  setAlignItems(&div, PILATES_ALIGN_END);

  Node div1 = divNode();
  setNodeSize(&div1, 4, 8);
  div1.id = 1;

  Node children[1] = {div1};
  div.children = children;
  div.num_children = 1;

  layoutNodes(&div, asciiMeasureText);

  printNode(&div, printf);

  printf("\n");

  asciiRender(&div);

  return 0;
}
