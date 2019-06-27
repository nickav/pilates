#include "pilates.h"

#include "debug.h"

#include <stdio.h>

int main() {
  Node div = divNode();
  div.width = 32;
  div.height = 14;

  setFlexDirection(&div, PILATES_COLUMN);
  //setJustifyContent(&div, PILATES_ALIGN_CENTER);
  //setAlignItems(&div, PILATES_ALIGN_CENTER);

  Node div1 = divNode();
  div1.width = 4;
  div1.height = 4;
  div1.id = 1;

  Node div2 = divNode();
  div2.width = 4;
  div2.height = 4;
  div2.id = 2;

  Node children[2] = {div1, div2};
  //  Node children[2] = {textNode("Hiiiiiiiiiiiiiiiiiiiii"), textNode("yooooooooooooo")};
  div.children = children;
  div.num_children = 2;

  layoutNodes(&div, asciiMeasureText);

  printNode(&div, printf);

  printf("\n");

  asciiRender(&div);

  return 0;
}
