#include "test.h"

void threeColumnLayout() {
  Node threecol[] = {makeDivNode(4, 4), makeDivNode(4, 4), makeDivNode(4, 4)};

  Node root = makeDivNode(24, 16, threecol, ArrayCount(threecol));

  setFlexDirection(&root, PILATES_ROW);
  setAlignItems(&root, PILATES_STRETCH);

  setFlexGrow(&threecol[1], 1);
  setAlignItems(&threecol[1], PILATES_STRETCH);

  Node expectedThreeCol[] = {mk(0, 0, 4, 16), mk(4, 0, 16, 16),
                             mk(20, 0, 4, 16)};

  Node expected = mk(0, 0, 24, 16, expectedThreeCol, 3);

  RunTest(&root, &expected);
}

int main() {
  threeColumnLayout();

  if (tests_failed == 0) {
    printf("\nAll %d test(s) passed! 🧘\n", tests_run);
  } else {
    printf("\n%d test(s) failed!\n", tests_failed);
  }

  return tests_failed > 0;
}
