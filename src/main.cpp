#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Max(a, b) (a > b ? a : b)
#define Assert(expr) if (!(expr)) { printf("Assertion failed at line %d\n", __LINE__); *(int *) 0 = 0; }

enum NodeType { NodeType_DIV, NodeType_TEXT, NodeType_COUNT };

enum PropType {
  PropType_FLEX_DIRECTION,
  PropType_JUSTIFY_CONTENT,
  PropType_COUNT
};

#define FLEX_DIR_ROW 0
#define FLEX_DIR_COLUMN 1

#define JUSTIFY_CONTENT_NORMAL 0
#define JUSTIFY_CONTENT_CENTER 1
#define JUSTIFY_CONTENT_FLEX_END 2

struct Node {
  NodeType type;
  int props[PropType_COUNT];

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

void setNodeSize(Node *node, int width, int height) {
  node->width = width;
  node->height = height;
}

void setFlexDirection(Node *node, int value) {
  node->props[PropType_FLEX_DIRECTION] = value;
}

void setJustifyContent(Node *node, int value) {
  node->props[PropType_JUSTIFY_CONTENT] = value;
}

void printNode(Node *node, int indent = 0) {
  for (int i = 0; i < indent; i++) {
    printf(" ");
  }

  if (node->type == NodeType_TEXT) {
    printf("TextNode: %s - %d %d %d %d", node->text, node->x, node->y,
           node->width, node->height);
  } else {
    printf("DivNode: %d %d %d %d\n", node->x, node->y, node->width,
           node->height);

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

int updatePrimaryAxisSize(int axis, Node *node) {
  int total = 0;

  auto *nodes = node->children;
  int n = node->num_children;

  if (axis == FLEX_DIR_ROW) {
    for (int i = 0; i < n; i++) {
      total += nodes[i].width;
    }

    node->width = Max(total, node->width);
  }

  if (axis == FLEX_DIR_COLUMN) {

    for (int i = 0; i < n; i++) {
      total += nodes[i].height;
    }

    node->height = Max(total, node->height);
  }

  return total;
}

int calcPrimaryOffset(int value, int childrenSize, int parentSize) {
  switch (value) {
  case JUSTIFY_CONTENT_CENTER:
    return (parentSize - childrenSize) / 2;
  case JUSTIFY_CONTENT_FLEX_END:
    return parentSize - childrenSize;
  case JUSTIFY_CONTENT_NORMAL:
  default:
    return 0;
  }
}

void layoutNodes(Node *node, MeasureTextFn *measureText) {

  if (node->type == NodeType_TEXT) {
    measureText(0, node->text, &node->width, &node->height);
    return;
  }

  if (node->type == NodeType_DIV) {
    // compute dimensions on each node
    for (int i = 0; i < node->num_children; i++) {
      Node *child = &node->children[i];
      layoutNodes(child, measureText);
      node->height = Max(node->height, child->height);
    }

    auto flexDir = node->props[PropType_FLEX_DIRECTION];
    auto justifyContent = node->props[PropType_JUSTIFY_CONTENT];

    // compute total size of children
    int childrenPrimarySize = updatePrimaryAxisSize(flexDir, node);

    // update final x position of children
    int primaryOffset = calcPrimaryOffset(justifyContent, childrenPrimarySize, flexDir == FLEX_DIR_ROW ? node->width : node->height);

    // TODO: y and height
    int xPos = primaryOffset;
    for (int i = 0; i < node->num_children; i++) {
      Node *child = &node->children[i];
      if (flexDir == FLEX_DIR_ROW) {
        child->x = xPos;
        xPos += child->width;
      } else {
        child->y = xPos;
        xPos += child->height;
      }
    }

    return;
  }
}

void asciiRenderNode(Node *node, char *output, int width, int height) {
  // we have the node's position
  int index = node->y * width + node->x;
  Assert(index < width * height);

  if (node->type == NodeType_TEXT) {
    memcpy(&output[index], node->text, strlen(node->text));
  } else {
    for (int i = 0; i < node->num_children; i++) {
      Node *child = &node->children[i];
      asciiRenderNode(child, output, width, height);
    }
  }
}

void asciiRender(Node *node) {
  int width = node->width;
  int height = node->height;

  int buf_len = width * height + 1;
  char *buffer = (char *)malloc(sizeof(char) * (buf_len));
  memset(buffer, ' ', buf_len);
  buffer[buf_len] = '\0';

  asciiRenderNode(node, buffer, width, height);

  printf("width: %d\nheight: %d\n", width, height);

  // print
  for (int i = 0; i < width; i++)
    printf("-");
  printf("\n");

  for (int i = 0; i < height; i++) {
    fwrite(&buffer[i * width], 1, width, stdout);
    printf("\n");
  }

  for (int i = 0; i < width; i++)
    printf("-");

  free(buffer);
}

int main() {
  Node div = makeDivNode();
  div.height = 12;
  div.width = 32;
  setFlexDirection(&div, FLEX_DIR_COLUMN);
  setJustifyContent(&div, JUSTIFY_CONTENT_CENTER);

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
