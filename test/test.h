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

Node mk(float x, float y, float width, float height, Node *children = NULL,
        int num_children = 0) {
  return Node{
      .x = x,
      .y = y,
      .width = width,
      .height = height,
      .children = children,
      .num_children = num_children,
  };
}

void layoutAndPrint(Node *root) {
  root->id = 0;
  layoutNodes(root, asciiMeasureText);
  printf("\n");
  printNode(root, true, printf);
  printf("\n");
  asciiRender(root);
}

static int tests_failed = 0;
static int tests_run = 0;

#define AssertEquals(a, b)                                                     \
  if (!(nodeBoundsEqualsRecursive(a, b))) {                                    \
    PILATES_PRINT_FUNC("Test '%s' failed on line %d\n", __FUNCTION__,          \
                       __LINE__);                                              \
    tests_failed++;                                                            \
    PILATES_PRINT_FUNC("\n");                                                  \
    PILATES_PRINT_FUNC("Expected:\n");                                         \
    if (a)                                                                     \
      layoutAndPrint(a);                                                       \
    PILATES_PRINT_FUNC("\n\nGot:\n");                                          \
    if (b)                                                                     \
      layoutAndPrint(b);                                                       \
    PILATES_PRINT_FUNC("\n");                                                  \
  }

#define RunTest(a, b)                                                          \
  tests_run++;                                                                 \
  layoutNodes(a, asciiMeasureText);                                            \
  AssertEquals(a, b);
