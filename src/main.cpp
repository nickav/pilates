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
  Node children[] = {makeDivNode(4, 4), makeDivNode(4, 4), makeDivNode(4, 4),
                     makeDivNode(4, 4)};
  Node root = makeDivNode(24, 32, children, ArrayCount(children));
  root.id = 0;

  setFlexDirection(&root, PILATES_ROW);
  setJustifyContent(&root, PILATES_SPACE_BETWEEN);
  setAlignItems(&root, PILATES_ALIGN_CENTER);

  layoutAndPrint(&root);
}

void example2() {
  Node childChildren[] = {makeDivNode(6, 4)};
  Node children[] = {makeDivNode(8, 16, childChildren, ArrayCount(childChildren))};
  Node root = makeDivNode(32, 32, children, ArrayCount(children));
  root.id = 0;

  setFlexDirection(&root, PILATES_ROW);
  setJustifyContent(&root, PILATES_ALIGN_CENTER);
  setAlignItems(&root, PILATES_ALIGN_END);

  Node *div1 = &children[0];
  setFlexDirection(div1, PILATES_ROW);
  setJustifyContent(div1, PILATES_ALIGN_CENTER);
  setAlignItems(div1, PILATES_ALIGN_END);

  layoutAndPrint(&root);
}

void example3() {
  Node children[] = {makeDivNode(4, 4), makeDivNode(4, 4)};
  Node root = makeDivNode(24, 16, children, ArrayCount(children));
  root.id = 0;

  setFlexDirection(&root, PILATES_ROW);
  setFlexGrow(&root.children[0], 1);
  // setFlexGrow(&root.children[1], 1);
  layoutAndPrint(&root);
}

int main() {
  example3();
  return 0;
}
