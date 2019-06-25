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
}

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
  };
}

Node makeDivNode() {
  return Node{
      .type = NodeType_DIV,
  };
}

void printNode(Node *node, int indent = 0) {
  for (int i = 0; i < indent; i++) {
    printf(" ");
  }

  if (node->type == NodeType_TEXT) {
    printf("TextNode: %s", node->text);
  } else {
    printf("DivNode:\n");

    for (int i = 0; i < node->num_children; i++) {
      printNode(&node->children[i], indent + 2);
    }
  }

  printf("\n");
}

int main() {
  Node div = makeDivNode();
  Node children[2] = {makeTextNode("Hello"), makeTextNode("world!")};
  div.children = children;
  div.num_children = 2;

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
