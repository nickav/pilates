#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// We need a way to measure text
// Width: sum of the widths of all the characters
// Height: line height * # of newlines (post-wrapped)

// Every node has a width and height
// They can be provided or computed
// We want to calculate the pixel positions (upper left offset) and height/width
// Padding and margin

// Built-in Macros
#define Max(a, b) (a > b ? a : b)
#define Min(a, b) (a < b ? a : b)

#define Assert(expr)                                                           \
  if (!(expr)) {                                                               \
    printf("Assert failed in function '%s' on line: %d\n", __FUNCTION__,       \
           __LINE__);                                                          \
    *(volatile int *)0 = 0;                                                    \
  }

// Library Macros

// flex-direction
#define PILATES_ROW 0
#define PILATES_COLUMN 1

// both for justify-content and align-items
#define PILATES_ALIGN_NORMAL 0
#define PILATES_ALIGN_CENTER 1
#define PILATES_ALIGN_END 2

enum NodeType { DIV, TEXT, NodeType_COUNT };

enum PropType { FLEX_DIRECTION, JUSTIFY_CONTENT, ALIGN_ITEMS, PropType_COUNT };

struct Node {
  NodeType type;
  int props[PropType_COUNT];

  Node *children;
  int num_children;

  Node *parent;
  char *text;

  float x, y;
  float width, height;
};

#define ForEachChild(Parent, Body)                                             \
  for (int i = 0; i < Parent->num_children; i++) {                             \
    auto *child = &Parent->children[i];                                        \
    Body                                                                       \
  }

Node textNode(char *text) { return Node{.type = TEXT, .text = text}; }

Node divNode() { return Node{.type = DIV}; }

void setNodeSize(Node *node, int width, int height) {
  node->width = width;
  node->height = height;
}

#define createPropSetter(PropName, PropType)                                   \
  void PropName(Node *node, int value) { node->props[PropType] = value; }

createPropSetter(setAlignItems, ALIGN_ITEMS);
createPropSetter(setFlexDirection, FLEX_DIRECTION);
createPropSetter(setJustifyContent, JUSTIFY_CONTENT);

#undef createPropSetter

void printNode(Node *node, int indent = 0) {
  for (int i = 0; i < indent; i++) {
    printf(" ");
  }

  if (node->type == TEXT) {
    printf("TextNode: %s - %f %f %f %f", node->text, node->x, node->y,
           node->width, node->height);
  } else {
    printf("DivNode: %f %f %f %f\n", node->x, node->y, node->width,
           node->height);

    for (int i = 0; i < node->num_children; i++) {
      printNode(&node->children[i], indent + 2);
    }
  }

  printf("\n");
}

#define MEASURE_TEXT(Name)                                                     \
  void Name(int font_id, char *text, float *width, float *height)
typedef MEASURE_TEXT(MeasureTextFn);

MEASURE_TEXT(monoSquareMeasure) {
  *width = strlen(text);
  *height = 1;
}

void updateNodeSize(int axis, Node *node, float &primaryAxisSize) {
  primaryAxisSize = 0;
  float secondaryAxisSize = 0;

  if (axis == PILATES_ROW) {
    ForEachChild(node, {
      primaryAxisSize += child->width;
      secondaryAxisSize = Max(secondaryAxisSize, child->height);
    });

    node->width = Max(primaryAxisSize, node->width);
    node->height = Max(secondaryAxisSize, node->height);
  }

  if (axis == PILATES_COLUMN) {
    ForEachChild(node, {
      primaryAxisSize += child->height;
      secondaryAxisSize = Max(secondaryAxisSize, child->width);
    });

    node->height = Max(primaryAxisSize, node->height);
    node->width = Max(secondaryAxisSize, node->width);
  }
}

int calcAxisOffset(int value, int childrenSize, int parentSize) {
  switch (value) {
  case PILATES_ALIGN_CENTER:
    return (parentSize - childrenSize) / 2;
  case PILATES_ALIGN_END:
    return parentSize - childrenSize;
  case PILATES_ALIGN_NORMAL:
  default:
    return 0;
  }
}

void layoutNodes(Node *node, MeasureTextFn *measureText) {

  if (node->type == TEXT) {
    measureText(0, node->text, &node->width, &node->height);
    return;
  }

  if (node->type == DIV) {
    // compute dimensions on each node
    for (int i = 0; i < node->num_children; i++) {
      Node *child = &node->children[i];
      layoutNodes(child, measureText);
      node->height = Max(node->height, child->height);
    }

    auto flexDir = node->props[FLEX_DIRECTION];
    auto justifyContent = node->props[JUSTIFY_CONTENT];
    auto alignItems = node->props[ALIGN_ITEMS];

    // compute total size of children
    float childrenPrimarySize;
    updateNodeSize(flexDir, node, childrenPrimarySize);

    // update final x position of children
    float primaryOffset =
        calcAxisOffset(justifyContent, childrenPrimarySize,
                       flexDir == PILATES_ROW ? node->width : node->height);
    float secondaryParentSize =
        flexDir == PILATES_ROW ? node->height : node->width;

    // TODO: y and height
    float pPos = primaryOffset;
    for (int i = 0; i < node->num_children; i++) {
      Node *child = &node->children[i];
      if (flexDir == PILATES_ROW) {
        child->x = pPos;
        pPos += child->width;
        child->y =
            calcAxisOffset(alignItems, child->height, secondaryParentSize);
      } else {
        child->y = pPos;
        pPos += child->height;
        child->x =
            calcAxisOffset(alignItems, child->width, secondaryParentSize);
      }
    }

    return;
  }
}

void asciiRenderNode(Node *node, char *output, int width, int height) {
  // we have the node's position
  int index = node->y * width + node->x;
  Assert(index < width * height);

  if (node->type == TEXT) {
    memcpy(&output[index], node->text, strlen(node->text));
  } else {
    ForEachChild(node, { asciiRenderNode(child, output, width, height); });
  }
}

void asciiRender(Node *node) {
  int width = (int)node->width;
  int height = (int)node->height;

  // alloc temporary storage
  int buf_len = width * height + 1;
  char *buffer = (char *)malloc(sizeof(char) * (buf_len));
  memset(buffer, ' ', buf_len);
  buffer[buf_len] = '\0';

  asciiRenderNode(node, buffer, width, height);

  printf("width: %d\nheight: %d\n", width, height);

  // print the grid
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

#undef ForEachChild

int main() {
  Node div = divNode();
  div.height = 13;
  div.width = 32;
  setFlexDirection(&div, PILATES_ROW);
  setJustifyContent(&div, PILATES_ALIGN_CENTER);
  setAlignItems(&div, PILATES_ALIGN_CENTER);

  Node children[2] = {textNode("Hi"), textNode("world!")};
  div.children = children;
  div.num_children = 2;

  layoutNodes(&div, monoSquareMeasure);

  printNode(&div);

  printf("\n");

  asciiRender(&div);

  return 0;
}
