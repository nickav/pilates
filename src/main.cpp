#include "pilates.h"

#include "debug.h"

#include <stdio.h>

int main() {
  Node div = divNode();
  div.height = 13;
  div.width = 32;
  setFlexDirection(&div, PILATES_ROW);
  setJustifyContent(&div, PILATES_ALIGN_CENTER);
  setAlignItems(&div, PILATES_ALIGN_CENTER);

  Node children[2] = {textNode("Hi"), textNode("world!")};
  div.children = children;
  div.num_children = 2;

  layoutNodes(&div, asciiMeasureText);

  printNode(&div, printf);

  printf("\n");

  asciiRender(&div);

  return 0;
}
