#include "test.h"

// alignment and spacing
Test(alignmentAndSpacing) {
  Node children[] = {makeDivNode(4, 4), makeDivNode(4, 4), makeDivNode(4, 4),
                     makeDivNode(4, 4)};
  Node root = makeDivNode(40, 16, children, ArrayCount(children));

  setFlexDirection(&root, PILATES_ROW);

  // case 1:
  {
    setJustifyContent(&root, PILATES_SPACE_BETWEEN);
    setAlignItems(&root, PILATES_ALIGN_CENTER);

    Node expectedChildren[] = {mk(0, 6, 4, 4), mk(12, 6, 4, 4), mk(24, 6, 4, 4), mk(36, 6, 4, 4)};
    Node expected = mk(0, 0, 40, 16, expectedChildren, ArrayCount(expectedChildren));

    layout(&root);
    AssertEquals(&root, &expected);
  }

  // case 2:
  {
    setJustifyContent(&root, PILATES_SPACE_AROUND);
    setAlignItems(&root, PILATES_ALIGN_START);

    Node expectedChildren[] = {mk(3, 0, 4, 4), mk(13, 0, 4, 4), mk(23, 0, 4, 4), mk(33, 0, 4, 4)};
    Node expected = mk(0, 0, 40, 16, expectedChildren, ArrayCount(expectedChildren));

    layout(&root);
    AssertEquals(&root, &expected);
  }

  // case 3:
  {
    setJustifyContent(&root, PILATES_SPACE_EVENLY);
    setAlignItems(&root, PILATES_ALIGN_END);

    Node expectedChildren[] = {mk(4.8, 12, 4, 4), mk(13.6, 12, 4, 4), mk(22.4, 12, 4, 4), mk(31.2, 12, 4, 4)};
    Node expected = mk(0, 0, 40, 16, expectedChildren, ArrayCount(expectedChildren));

    layout(&root);
    // TODO: fix floating point comparisons
    //AssertEquals(&root, &expected);
  }

  return true;
}

Test(threeColumnLayout) {
  Node threecol[] = {makeDivNode(4, 4), makeDivNode(4, 4), makeDivNode(4, 4)};

  Node root = makeDivNode(24, 16, threecol, ArrayCount(threecol));

  setFlexDirection(&root, PILATES_ROW);
  setAlignItems(&root, PILATES_STRETCH);

  setFlexGrow(&threecol[1], 1);
  setAlignItems(&threecol[1], PILATES_STRETCH);

  Node expectedThreeCol[] = {mk(0, 0, 4, 16), mk(4, 0, 16, 16),
                             mk(20, 0, 4, 16)};

  Node expected =
      mk(0, 0, 24, 16, expectedThreeCol, ArrayCount(expectedThreeCol));

  layout(&root);
  AssertEquals(&root, &expected);

  return true;
}

Test(parentHeightFromChildren) {
  Node items[] = {makeDivNode(8, 8)};
  Node root = makeDivNode(0, 0, items, ArrayCount(items));

  Node expectedChildren[] = {mk(0, 0, 8, 8)};
  Node expected =
      mk(0, 0, 8, 8, expectedChildren, ArrayCount(expectedChildren));

  layout(&root);
  AssertEquals(&root, &expected);

  return true;
}

TestFunc *tests[] = {alignmentAndSpacing, threeColumnLayout,
                     parentHeightFromChildren, NULL};

int main() {
  int totalTests = 0;
  int numPassed = runTests(tests, totalTests);
  int numFailed = totalTests - numPassed;

  if (numPassed == totalTests) {
    printf("\nAll test(s) passed! 🧘\n");
  } else {
    printf("\n%d test(s) failed!\n", numFailed);
  }

  return numFailed > 0;
}
