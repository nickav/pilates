#include "pilates.h"

#include "debug.h"

#include <stdio.h>

// test helpers
Node mkdiv(float x, float y, float width, float height, Node *children,
           int num_children) {
  static int id = 1;
  return Node{
      .id = id++,
      .x = x,
      .y = y,
      .width = width,
      .height = height,
      .children = children,
      .num_children = num_children,
  };
}

Node mkdiv(float x, float y, float width, float height) {
  return mkdiv(x, y, width, height, NULL, 0);
}

Node mkdiv(float width, float height) {
  return mkdiv(0, 0, width, height, NULL, 0);
}

#define mkdivp(x, y, width, height, children)                                  \
  mkdiv(x, y, width, height, children, ArrayCount(children))

void printAndRender(Node *root, bool verbose) {
  printf("\n");
  printNode(root, verbose, printf);
  printf("\n");

  root->id = 0;
  asciiRender(root);
}

inline void layout(Node *root) { layoutNodes(root, asciiMeasureText); }

// test utilities
inline bool AssertBoundsEqualsFn(Node *result, Node *expected,
                                 char *functionName, int lineNum) {
  if (!(nodeBoundsEqualsRecursive(result, expected))) {
    PILATES_PRINT_FUNC("Test '%s' failed on line %d:\n", functionName, lineNum);
    PILATES_PRINT_FUNC("\n");
    PILATES_PRINT_FUNC("Expected:\n");
    if (expected)
      printAndRender(expected, false);
    else
      printf("NULL\n");
    PILATES_PRINT_FUNC("\n\nGot:\n");
    if (result)
      printAndRender(result, true);
    else
      printf("NULL\n");
    PILATES_PRINT_FUNC("\n");
    return false;
  }

  return true;
}

#define AssertEquals(result, expected)                                         \
  if (!AssertBoundsEqualsFn(result, expected, (char *)__FUNCTION__,            \
                            __LINE__)) {                                       \
    return false;                                                              \
  }

#define Test(name) bool name()
typedef Test(TestFunc);

int runTests(TestFunc **tests, int &totalTests) {
  int testsPassed = 0;

  totalTests = 0;

  while (tests && tests[0]) {
    if (tests[0]()) {
      testsPassed++;
    }
    tests++;
    totalTests++;
  }

  return testsPassed;
}
