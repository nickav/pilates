#include "pilates.h"

#include "debug.h"

#include <stdio.h>

int main() {
  Node div = divNode();
  div.height = 13;
  div.width = 16;

  setFlexDirection(&div, PILATES_COLUMN);
  //setJustifyContent(&div, PILATES_ALIGN_CENTER);
  //setAlignItems(&div, PILATES_ALIGN_CENTER);

  Node children[2] = {textNode("Hii hey whats up world"),
                      textNode("Lorem ipsum text hey there this")};
  //  Node children[2] = {textNode("Hiiiiiiiiiiiiiiiiiiiii"), textNode("yooooooooooooo")};
  div.children = children;
  div.num_children = 2;

  layoutNodes(&div, asciiMeasureText);

  printNode(&div, printf);

  printf("\n");

  asciiRender(&div);

  return 0;
}
