#include "test.h"

Test(alignmentAndSpacing) {
  Node children[] = {mkdiv(4, 4), mkdiv(4, 4), mkdiv(4, 4), mkdiv(4, 4)};
  Node root = mkdivp(0, 0, 40, 16, children);

  setFlexDirection(&root, PILATES_ROW);

  // case 1:
  {
    setJustifyContent(&root, PILATES_SPACE_BETWEEN);
    setAlignItems(&root, PILATES_ALIGN_CENTER);

    Node ec1[] = {mkdiv(0, 6, 4, 4), mkdiv(12, 6, 4, 4), mkdiv(24, 6, 4, 4),
                  mkdiv(36, 6, 4, 4)};
    Node e = mkdivp(0, 0, 40, 16, ec1);

    layout(&root);
    AssertNodeEquals(&root, &e);
  }

  // case 2:
  {
    setJustifyContent(&root, PILATES_SPACE_AROUND);
    setAlignItems(&root, PILATES_ALIGN_START);

    Node ec1[] = {mkdiv(3, 0, 4, 4), mkdiv(13, 0, 4, 4), mkdiv(23, 0, 4, 4),
                  mkdiv(33, 0, 4, 4)};
    Node e = mkdivp(0, 0, 40, 16, ec1);

    layout(&root);
    AssertNodeEquals(&root, &e);
  }

  // case 3:
  {
    // TODO: fix floating point comparisons
    root.width = 26;
    setJustifyContent(&root, PILATES_SPACE_EVENLY);
    setAlignItems(&root, PILATES_ALIGN_END);

    Node ec1[] = {mkdiv(2, 12, 4, 4), mkdiv(8, 12, 4, 4), mkdiv(14, 12, 4, 4),
                  mkdiv(20, 12, 4, 4)};
    Node e = mkdivp(0, 0, 26, 16, ec1);

    layout(&root);
    AssertNodeEquals(&root, &e);
  }

  return true;
}

Test(columnLayout) {
  Node childChildren[] = {mkdiv(6, 4)};
  Node children[] = {mkdivp(0, 0, 8, 16, childChildren)};
  Node root = mkdivp(0, 0, 32, 32, children);

  setFlexDirection(&root, PILATES_COLUMN);
  setJustifyContent(&root, PILATES_ALIGN_CENTER);
  setAlignItems(&root, PILATES_ALIGN_END);

  Node *div1 = &children[0];
  setFlexDirection(div1, PILATES_ROW);
  setJustifyContent(div1, PILATES_ALIGN_CENTER);
  setAlignItems(div1, PILATES_ALIGN_END);

  // expected
  Node ec2[] = {mkdiv(25, 20, 6, 4)};
  Node ec1[] = {mkdivp(24, 8, 8, 16, ec2)};
  Node e = mkdivp(0, 0, 32, 32, ec1);

  layout(&root);
  AssertNodeEquals(&root, &e);

  return true;
}

Test(flexGrow) {
  Node children[] = {mkdiv(4, 4), mkdiv(4, 4)};
  Node root = mkdivp(0, 0, 24, 16, children);

  setFlexDirection(&root, PILATES_ROW);
  setFlexGrow(&root.children[0], 1);

  // case 1:
  // one child with flex, one without
  {
    Node ec1[] = {mkdiv(0, 0, 20, 4), mkdiv(20, 0, 4, 4)};
    Node e = mkdivp(0, 0, 24, 16, ec1);
    layout(&root);
    AssertNodeEquals(&root, &e);
  }

  // case 2:
  // both children with flex
  {
    setFlexGrow(&root.children[1], 1);

    Node ec1[] = {mkdiv(0, 0, 12, 4), mkdiv(12, 0, 12, 4)};
    Node e = mkdivp(0, 0, 24, 16, ec1);
    layout(&root);
    AssertNodeEquals(&root, &e);
  }

  return true;
}

Test(threeColumnLayout) {
  Node threecol[] = {mkdiv(4, 4), mkdiv(4, 4), mkdiv(4, 4)};

  Node root = mkdivp(0, 0, 24, 16, threecol);

  setFlexDirection(&root, PILATES_ROW);
  setAlignItems(&root, PILATES_STRETCH);

  setFlexGrow(&threecol[1], 1);
  setAlignItems(&threecol[1], PILATES_STRETCH);

  Node ec1[] = {mkdiv(0, 0, 4, 16), mkdiv(4, 0, 16, 16), mkdiv(20, 0, 4, 16)};
  Node e = mkdivp(0, 0, 24, 16, ec1);

  layout(&root);
  AssertNodeEquals(&root, &e);

  return true;
}

Test(simpleThreeCol) {
  Node threecol[] = {mkdiv(4, 4), mkdiv(4, 4), mkdiv(4, 4)};
  Node parent[] = {mkdivp(0, 0, 4, 4, threecol)};
  Node root = mkdivp(0, 0, 24, 16, parent);

  setFlexDirection(&root, PILATES_ROW);
  setAlignItems(&root, PILATES_STRETCH);

  setFlexGrow(&parent[0], 1);
  // setAlignItems(&parent[0], PILATES_STRETCH);
  setFlexGrow(&threecol[1], 1);

  Node ec2[] = {mkdiv(0, 0, 4, 4), mkdiv(4, 0, 16, 4), mkdiv(20, 0, 4, 4)};
  Node ec1[] = {mkdivp(0, 0, 24, 16, ec2)};
  Node e = mkdivp(0, 0, 24, 16, ec1);

  layout(&root);
  AssertNodeEquals(&root, &e);

  return true;
}

Test(flexWrap) {
  float w = 7;
  float h = 8;
  Node items[] = {mkdiv(w, h), mkdiv(w, h), mkdiv(w, h), mkdiv(w, h)};
  Node root = mkdivp(0, 0, 24, 24, items);

  setFlexWrap(&root, PILATES_WRAP);

  // normal wrapping
  {
    Node ec1[] = {mkdiv(0, 0, w, h), mkdiv(7, 0, w, h), mkdiv(14, 0, w, h),
                  mkdiv(0, 12, w, h)};
    Node e = mkdivp(0, 0, 24, 24, ec1);
    layout(&root);
    AssertNodeEquals(&root, &e);
  }

  // wrap with grow
  {
    Node ec1[] = {mkdiv(0, 0, 8, h), mkdiv(8, 0, 8, h), mkdiv(16, 0, 8, h),
                  mkdiv(0, 12, 24, h)};
    Node e = mkdivp(0, 0, 24, 24, ec1);

    for (int i = 0; i < ArrayCount(items); i++) {
      setFlexGrow(&items[i], 1);
    }

    layout(&root);
    AssertNodeEquals(&root, &e);
  }

  return true;
}

Test(parentHeightFromChildren) {
  Node items[] = {mkdiv(8, 8)};
  Node root = mkdivp(0, 0, 0, 0, items);

  Node ec1[] = {mkdiv(0, 0, 8, 8)};
  Node e = mkdivp(0, 0, 8, 8, ec1);

  layout(&root);
  AssertNodeEquals(&root, &e);

  return true;
}

Test(multiPass) {
  Node items[] = {mkdiv(4, 4)};
  Node root = mkdivp(0, 0, 16, 16, items);

  // first pass:
  setFlexGrow(&items[0], 1);
  setFlexDirection(&root, PILATES_ROW);
  layout(&root);

  AssertEquals(items[0].width, 16);
  AssertEquals(items[0].height, 4);

  // second pass:
  items[0].width = 4;
  items[0].height = 4;
  setFlexDirection(&root, PILATES_COLUMN);
  layout(&root);

  AssertEquals(items[0].width, 4);
  AssertEquals(items[0].height, 16);

  return true;
}

Test(growToTextSize) {
  Node items[] = {mktext("hello")};
  Node root = mkdivp(0, 0, 0, 0, items);

  Node ec1[] = {mkdiv(0, 0, 5, 1)};
  Node e = mkdivp(0, 0, 5, 1, ec1);

  layout(&root);
  AssertNodeEquals(&root, &e);

  return true;
}

Test(wrapText) {
  {
    Node items[] = {mktext("hello world")};
    Node root = mkdivp(0, 0, 5, 0, items);

    Node ec1[] = {mkdiv(0, 0, 5, 2)};
    Node e = mkdivp(0, 0, 5, 2, ec1);

    layout(&root);
    AssertNodeEquals(&root, &e);
  }

  {
    Node items[] = {mktext("hi hi hi hello")};
    Node root = mkdivp(0, 0, 5, 0, items);

    Node ec1[] = {mkdiv(0, 0, 5, 3)};
    Node e = mkdivp(0, 0, 5, 3, ec1);

    layout(&root);
    AssertNodeEquals(&root, &e);
  }

  return true;
}

TestFunc *tests[] = {alignmentAndSpacing,
                     threeColumnLayout,
                     parentHeightFromChildren,
                     columnLayout,
                     simpleThreeCol,
                     flexGrow,
                     flexWrap,
                     multiPass,
                     growToTextSize,
                     wrapText,
                     NULL};

int main() {
  int totalTests = 0;
  int numPassed = runTests(tests, totalTests);
  int numFailed = totalTests - numPassed;

  if (numPassed == totalTests) {
    printf("\n%d test(s) passed! 🧘\n", numPassed);
  } else {
    printf("\n%d test(s) failed!\n", numFailed);
  }

  return numFailed;
}
