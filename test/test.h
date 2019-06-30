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

void printAndRender(Node *root, bool verbose) {
  printf("\n");
  printNode(root, verbose, printf);
  printf("\n");

  root->id = 0;
  asciiRender(root);
}

static int tests_failed = 0;
static int tests_run = 0;

inline bool AssertEquals(Node *result, Node *expected, char *functionName,
                         int lineNum) {

  if (!(nodeBoundsEqualsRecursive(result, expected))) {
    PILATES_PRINT_FUNC("Test '%s' failed on line %d\n", functionName, lineNum);
    tests_failed++;
    PILATES_PRINT_FUNC("\n");
    PILATES_PRINT_FUNC("Expected:\n");
    if (expected)
      printAndRender(expected, false);
    PILATES_PRINT_FUNC("\n\nGot:\n");
    if (result)
      printAndRender(result, true);
    PILATES_PRINT_FUNC("\n");
    return false;
  }

  return true;
}

#define RunTest(result, expected)                                              \
  tests_run++;                                                                 \
  layoutNodes(result, asciiMeasureText);                                       \
  AssertEquals(result, expected, (char *)__FUNCTION__, __LINE__);
