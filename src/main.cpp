#include <stdio.h>
#include <stdlib.h>
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

#define Max(a, b) (a > b ? a : b)

struct Node {
  NodeType type;
  Prop props[PropType_COUNT];

  Node *children;
  int num_children;

  Node *parent;
  char *text;

  int x, y;
  int width, height;
};

Node makeTextNode(char *text) {
  return Node{
      .type = NodeType_TEXT,
      .text = text,
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
    printf("TextNode: %s - %d %d %d %d", node->text, node->x,
           node->y, node->width, node->height);
  } else {
    printf("DivNode: %d %d %d %d\n", node->x, node->y,
           node->width, node->height);

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
    measureText(0, node->text, &node->width, &node->height);
  } else if (node->type == NodeType_DIV) {
    for (int i = 0; i < node->num_children; i++) {
      Node *child = &node->children[i];
      layoutNodes(child, measureText);
      node->width += child->width;
      node->height = Max(node->height, child->height);
    }

    int xPos = 0;
    for (int i = 0; i < node->num_children; i++) {
      Node *child = &node->children[i];
      child->x = xPos;
      xPos += child->width;
    }
  }
}

void asciiRenderNode(Node *node, char *output, int width, int height) {
  // we have the node's position
  int index = node->y * width + node->x;

  if (node->type == NodeType_TEXT) {
    strcpy(&output[index], node->text);
  } else {
    for (int i = 0; i < node->num_children; i++) {
      Node *child = &node->children[i];
      asciiRenderNode(child, output, width, height);
    }
  }
}

void asciiRender(Node *node) {
  int buf_len = node->width * node->height + 1;
  char *buffer = (char *)malloc(sizeof(char) * (buf_len));
  buffer[buf_len] = '\0';

  asciiRenderNode(node, buffer, node->width, node->height);

  printf("%s", buffer);
  free(buffer);
}

int main() {
  Node div = makeDivNode();
  Node children[2] = {makeTextNode("Hello "), makeTextNode("world!")};
  div.children = children;
  div.num_children = 2;

  layoutNodes(&div, monoSquareMeasure);

  printNode(&div);

  printf("\n");

  asciiRender(&div);

  return 0;
}

// We need a way to measure text
// Width: sum of the widths of all the characters
// Height: line height * # of newlines (post-wrapped)

// Every node has a width and height
// They can be provided or computed
// We want to calculate the pixel positions (upper left offset) and height/width
// Padding and margin

#undef ForEachChild
