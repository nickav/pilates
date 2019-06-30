#include "pilates.h"

#include "debug.h"

#include <stdio.h>

// test helpers
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

inline void layout(Node *root) { layoutNodes(root, asciiMeasureText); }

// test utilities
inline bool AssertBoundsEqualsFn(Node *result, Node *expected,
                                 char *functionName, int lineNum) {
  if (!(nodeBoundsEqualsRecursive(result, expected))) {
    PILATES_PRINT_FUNC("Test '%s' failed on line %d\n", functionName, lineNum);
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
