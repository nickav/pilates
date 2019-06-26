#pragma once

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

// Library Macros

// flex-direction
#define PILATES_ROW 0
#define PILATES_COLUMN 1

// both for justify-content and align-items
#define PILATES_ALIGN_NORMAL 0
#define PILATES_ALIGN_CENTER 1
#define PILATES_ALIGN_END 2

#define PILATES_MEASURE_TEXT(Name)                                             \
  void Name(int fontId, char *text, float *width, float *height)
typedef PILATES_MEASURE_TEXT(MeasureTextFunc);

enum NodeType { DIV, TEXT, NodeType_COUNT };

enum PropType { FLEX_DIRECTION, JUSTIFY_CONTENT, ALIGN_ITEMS, PropType_COUNT };

struct Node {
  NodeType type;
  int props[PropType_COUNT];
  Node *parent;

  float x, y;
  float width, height;

  // div nodes
  Node *children;
  int num_children;

  // text nodes
  char *text;
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

void layoutNodes(Node *node, MeasureTextFunc *measureText) {
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
