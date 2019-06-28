#include "pilates.h"

#include "debug.h"

#include <stdio.h>

Node makeDivNode(float width, float height, Node *children = NULL,
                 int num_children = 0) {
  static int id = 1;
  return Node{
    .id = id++,
    .width = width,
    .height = height,
    .children = children,
    .num_children = num_children,
  };
}

void layoutAndPrint(Node *root) {
  layoutNodes(root, asciiMeasureText);
  printNode(root, printf);
  printf("\n");
  asciiRender(root);
}

void example1() {
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

  layoutAndPrint(&div);
}

void example2() {
  Node div = divNode();
  div.width = 32;
  div.height = 32;

  setFlexDirection(&div, PILATES_ROW);
  setJustifyContent(&div, PILATES_ALIGN_CENTER);
  setAlignItems(&div, PILATES_ALIGN_END);

  Node div1 = divNode();
  setNodeSize(&div1, 8, 16);
  div1.id = 1;

  setFlexDirection(&div1, PILATES_ROW);
  setJustifyContent(&div1, PILATES_ALIGN_CENTER);
  setAlignItems(&div1, PILATES_ALIGN_END);

  Node div2 = divNode();
  setNodeSize(&div2, 6, 4);
  div2.id = 2;

  Node div1Children[1] = {div2};
  div1.children = div1Children;
  div1.num_children = 1;

  Node children[1] = {div1};
  div.children = children;
  div.num_children = 1;

  layoutAndPrint(&div);
}

void example3() {
  Node children[] = {makeDivNode(4, 4), makeDivNode(4, 4)};
  Node root = makeDivNode(24, 16, children, ArrayCount(children));
  root.id = 0;

  setFlexDirection(&root, PILATES_ROW);
  setFlexGrow(&root.children[0], 1);
  //setFlexGrow(&root.children[1], 1);
  layoutAndPrint(&root);
}

int main() {
  example3();
  return 0;
}
