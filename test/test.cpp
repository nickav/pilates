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

    Node ec1[] = {mk(0, 6, 4, 4), mk(12, 6, 4, 4), mk(24, 6, 4, 4),
                  mk(36, 6, 4, 4)};
    Node e = mk(0, 0, 40, 16, ec1, ArrayCount(ec1));

    layout(&root);
    AssertEquals(&root, &e);
  }

  // case 2:
  {
    setJustifyContent(&root, PILATES_SPACE_AROUND);
    setAlignItems(&root, PILATES_ALIGN_START);

    Node ec1[] = {mk(3, 0, 4, 4), mk(13, 0, 4, 4), mk(23, 0, 4, 4),
                  mk(33, 0, 4, 4)};
    Node e = mk(0, 0, 40, 16, ec1, ArrayCount(ec1));

    layout(&root);
    AssertEquals(&root, &e);
  }

  // case 3:
  {
    setJustifyContent(&root, PILATES_SPACE_EVENLY);
    setAlignItems(&root, PILATES_ALIGN_END);

    Node ec1[] = {mk(4.8, 12, 4, 4), mk(13.6, 12, 4, 4), mk(22.4, 12, 4, 4),
                  mk(31.2, 12, 4, 4)};
    Node e = mk(0, 0, 40, 16, ec1, ArrayCount(ec1));

    layout(&root);
    // TODO: fix floating point comparisons
    // AssertEquals(&root, &e);
  }

  return true;
}

Test(columnLayout) {
  Node childChildren[] = {makeDivNode(6, 4)};
  Node children[] = {
      makeDivNode(8, 16, childChildren, ArrayCount(childChildren))};
  Node root = makeDivNode(32, 32, children, ArrayCount(children));

  setFlexDirection(&root, PILATES_COLUMN);
  setJustifyContent(&root, PILATES_ALIGN_CENTER);
  setAlignItems(&root, PILATES_ALIGN_END);

  Node *div1 = &children[0];
  setFlexDirection(div1, PILATES_ROW);
  setJustifyContent(div1, PILATES_ALIGN_CENTER);
  setAlignItems(div1, PILATES_ALIGN_END);

  // expected
  Node ec2[] = {mk(25, 20, 6, 4)};
  Node ec1[] = {mk(24, 8, 8, 16, ec2, ArrayCount(ec2))};
  Node e = mk(0, 0, 32, 32, ec1, ArrayCount(ec1));

  layout(&root);
  AssertEquals(&root, &e);

  return true;
}

Test(flexGrow) {
  Node children[] = {makeDivNode(4, 4), makeDivNode(4, 4)};
  Node root = makeDivNode(24, 16, children, ArrayCount(children));

  setFlexDirection(&root, PILATES_ROW);
  setFlexGrow(&root.children[0], 1);
  // setFlexGrow(&root.children[1], 1);

  // expected
  Node ec2[] = {mk(0, 0, 4, 4), mk(4, 0, 16, 4), mk(20, 0, 4, 4)};
  Node ec1[] = {mk(0, 0, 24, 16, ec2, ArrayCount(ec2))};
  Node e = mk(0, 0, 24, 16, ec1, ArrayCount(ec1));

  layout(&root);
  //AssertEquals(&root, &e);

  return true;
}

Test(threeColumnLayout) {
  Node threecol[] = {makeDivNode(4, 4), makeDivNode(4, 4), makeDivNode(4, 4)};

  Node root = makeDivNode(24, 16, threecol, ArrayCount(threecol));

  setFlexDirection(&root, PILATES_ROW);
  setAlignItems(&root, PILATES_STRETCH);

  setFlexGrow(&threecol[1], 1);
  setAlignItems(&threecol[1], PILATES_STRETCH);

  Node ec1[] = {mk(0, 0, 4, 16), mk(4, 0, 16, 16), mk(20, 0, 4, 16)};
  Node e = mk(0, 0, 24, 16, ec1, ArrayCount(ec1));

  layout(&root);
  AssertEquals(&root, &e);

  return true;
}

// simple 3 col layout
Test(simpleThreeCol) {
  Node threecol[] = {makeDivNode(4, 4), makeDivNode(4, 4), makeDivNode(4, 4)};
  Node parent[] = {makeDivNode(4, 4, threecol, ArrayCount(threecol))};
  Node root = makeDivNode(24, 16, parent, ArrayCount(parent));

  setFlexDirection(&root, PILATES_ROW);
  setAlignItems(&root, PILATES_STRETCH);

  setFlexGrow(&parent[0], 1);
  // setAlignItems(&parent[0], PILATES_STRETCH);
  setFlexGrow(&threecol[1], 1);

  Node ec2[] = {mk(0, 0, 4, 4), mk(4, 0, 16, 4), mk(20, 0, 4, 4)};
  Node ec1[] = {mk(0, 0, 24, 16, ec2, ArrayCount(ec2))};
  Node e = mk(0, 0, 24, 16, ec1, ArrayCount(ec1));

  layout(&root);
  AssertEquals(&root, &e);

  return true;
}

// wrapping example
Test(flexWrap) {
  float w = 7;
  float h = 8;
  Node items[] = {makeDivNode(w, h), makeDivNode(w, h), makeDivNode(w, h),
                  makeDivNode(w, h)};
  Node root = makeDivNode(24, 24, items, ArrayCount(items));

  setFlexWrap(&root, PILATES_WRAP);
  setFlexGrow(&items[0], 1);
  setFlexGrow(&items[1], 1);
  setFlexGrow(&items[2], 1);
  setFlexGrow(&items[3], 1);

  Node ec1[] = {mk(0, 0, w, h), mk(7, 0, w, h), mk(14, 0, w, h), mk(0, 12, w, h)};
  Node e = mk(0, 0, 24, 24, ec1, ArrayCount(ec1));

  layout(&root);
  AssertEquals(&root, &e);

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

TestFunc *tests[] = {alignmentAndSpacing,
                     threeColumnLayout,
                     parentHeightFromChildren,
                     columnLayout,
                     simpleThreeCol,
                     flexGrow,
                     flexWrap,
                     NULL};

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
