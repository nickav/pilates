#include "pilates.h"

#include "debug.h"

#include <stdio.h>

bool nodeBoundsEquals(Node *a, Node *b) {
  if (a == b) {
    return true;
  }

  if (a == NULL || b == NULL) {
    return false;
  }

  return (a->x == b->x && a->y == b->y && a->width == b->width &&
          a->height == b->height);
}

bool nodeBoundsEqualsRecursive(Node *a, Node *b) {
  if (!nodeBoundsEquals(a, b) || a->numChildren != b->numChildren) {
    return false;
  }

  ForEachChild(a, {
    Node *otherChild = &b->children[i];
    if (!nodeBoundsEqualsRecursive(child, otherChild)) {
      return false;
    }
  });

  return true;
}


// test helpers
Node mkdiv(float x, float y, float width, float height, Node *children,
           int numChildren) {
  static int id = 1;
  return Node{
      .id = id++,
      .x = x,
      .y = y,
      .width = width,
      .height = height,
      .children = children,
      .numChildren = numChildren,
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
inline bool AssertNodeBoundsEquals(Node *result, Node *expected,
                                   char *functionName, int lineNum) {
  if (!(nodeBoundsEqualsRecursive(result, expected))) {
    printf("Test '%s' failed on line %d:\n", functionName, lineNum);
    printf("\n");
    printf("Expected:\n");
    if (expected)
      printAndRender(expected, false);
    else
      printf("NULL\n");
    printf("\n\nGot:\n");
    if (result)
      printAndRender(result, true);
    else
      printf("NULL\n");
    printf("\n");
    return false;
  }

  return true;
}

#define AssertNodeEquals(result, expected)                                     \
  if (!AssertNodeBoundsEquals(result, expected, (char *)__FUNCTION__,          \
                              __LINE__)) {                                     \
    return false;                                                              \
  }

#define AssertEquals(a, b)                                                     \
  if (a != b) {                                                                \
    printf("Test '%s' AssertEquals failed on line %d\n",           \
                       __FUNCTION__, __LINE__);                                \
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
