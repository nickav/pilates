#include "pilates.h"

#include "debug.h"

#include <stdio.h>

int main() {
  Node div = divNode();
  div.height = 13;
  div.width = 16;
  setFlexDirection(&div, PILATES_ROW);
  setJustifyContent(&div, PILATES_ALIGN_CENTER);
  setAlignItems(&div, PILATES_ALIGN_CENTER);

  Node children[1] = {textNode("Hii hi hi hi hi hi hi hi hi world")};
  div.children = children;
  div.num_children = 1;

  layoutNodes(&div, asciiMeasureText);

  printNode(&div, printf);

  printf("\n");

  asciiRender(&div);

  return 0;
}
