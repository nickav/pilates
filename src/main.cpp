#include <stdio.h>
#include <string.h>

enum NodeType { NodeType_DIV, NodeType_TEXT, NodeType_COUNT };

enum PropType {
  PropValue_FLEX_DIRECTION,
  PropValue_JUSTIFY_CONTENT,
  PropType_COUNT
};

#define FLEX_DIR_ROW 0
#define FLEX_DIR_COLUMN 1

struct Prop {
  PropType key;
  int value;
};

struct Vec2 {
  int x;
  int y;
};

#define Max(a, b) (a > b ? a : b)

struct Node {
  NodeType type;
  Prop props[PropType_COUNT];

  Node *children;
  int num_children;

  Node *parent;
  char *text;

  Vec2 position;
  Vec2 size;
};

Node makeTextNode(char *text) {
  return Node{
      .type = NodeType_TEXT,
      .text = text,
      .num_children = 0,
      .size = Vec2{0, 0},
      .position = Vec2{0, 0},
  };
}

Node makeDivNode() {
  return Node{
      .type = NodeType_DIV,
      .size = Vec2{0, 0},
      .position = Vec2{0, 0},
  };
}

void printNode(Node *node, int indent = 0) {
  for (int i = 0; i < indent; i++) {
    printf(" ");
  }

  if (node->type == NodeType_TEXT) {
    printf("TextNode: %s - %d %d %d %d", node->text, node->position.x,
           node->position.y, node->size.x, node->size.y);
  } else {
    printf("DivNode: %d %d %d %d\n", node->position.x, node->position.y,
           node->size.x, node->size.y);

    for (int i = 0; i < node->num_children; i++) {
      printNode(&node->children[i], indent + 2);
    }
  }

  printf("\n");
}

#define MEASURE_TEXT(Name) void Name(int font_id, char *text, int *x, int *y)
typedef MEASURE_TEXT(MeasureTextFn);

MEASURE_TEXT(monoSquareMeasure) {
  *x = strlen(text);
  *y = 1;
}

void layoutNodes(Node *node, MeasureTextFn *measureText) {

  if (node->type == NodeType_TEXT) {
    measureText(0, node->text, &node->size.x, &node->size.y);
  } else if (node->type == NodeType_DIV) {
    for (int i = 0; i < node->num_children; i++) {
      Node *child = &node->children[i];
      layoutNodes(child, measureText);
      node->size.x += child->size.x;
      node->size.y = Max(node->size.y, child->size.y);
    }
  }
}

int main() {
  Node div = makeDivNode();
  Node children[2] = {makeTextNode("Hello"), makeTextNode("world!")};
  div.children = children;
  div.num_children = 2;

  layoutNodes(&div, monoSquareMeasure);

  printNode(&div);

  return 0;
}

// We need a way to measure text
// Width: sum of the widths of all the characters
// Height: line height * # of newlines (post-wrapped)

// Every node has a width and height
// They can be provided or computed
// We want to calculate the pixel positions (upper left offset) and height/width
// Padding and margin
