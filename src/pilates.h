#pragma once

// We need a way to measure text
// Width: sum of the widths of all the characters
// Height: line height * # of newlines (post-wrapped)

// Every node has a width and height
// They can be provided or computed
// We want to calculate the pixel positions (upper left offset) and height/width
// Padding and margin

#include <stdio.h>
#include <string.h>

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
  void Name(int fontId, char *text, int n, float *width, float *height)
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
  // primaryAxisSize is the total size of the children
  primaryAxisSize = 0;
  float secondaryAxisSize = 0;

  if (axis == PILATES_ROW) {
    ForEachChild(node, {
      primaryAxisSize += child->width;
      secondaryAxisSize = Max(secondaryAxisSize, child->height);
    });

    if (node->width == 0) {
      node->width = primaryAxisSize;
    }
    if (node->height == 0) {

      node->height = Max(secondaryAxisSize, node->height);
    }
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

int strpos(char *str, char search) {
  char *found = strchr(str, search);

  if (str) {
    return found - str;
  }

  return -1;
}

// Currently this only supports word-wrapping mode
int computeTextLineHeight(int fontId, MeasureTextFunc *measureText, char *text,
                          float maxWidth) {
  int nLines = 0;
  float lineWidth = 0;

  int n = strlen(text);
  for (int i = 0; i < n; i++) {
    int nextSpace = strpos(&text[i], ' ');

    if (nextSpace < 0)
      nextSpace = n;

    float width, height;
    measureText(fontId, &text[i], nextSpace - i, &width, &height);

    lineWidth += width;
    if (lineWidth > maxWidth) {
      nLines++;
      lineWidth = width;
    }

    i = nextSpace + 1;
  }

  return nLines;
}

// TODOs
// The height of the parent needs to grow based on the height of the wrapped
// children if no height is provided
// TODO: don't try to write outside of buffer range
// We need to wrap text
// The height is wrong

void layoutNodes(Node *node, MeasureTextFunc *measureText) {
  if (node->type == TEXT) {
    // given a max-width and a measuretext func and the text to measure,
    // and the font id, give us the height
    measureText(0, node->text, 0, &node->width, &node->height);
    return;
  }

  if (node->type == DIV) {
    // compute dimensions on each node
    for (int i = 0; i < node->num_children; i++) {
      Node *child = &node->children[i];
      layoutNodes(child, measureText);
      node->height = Max(node->height, child->height);
    }

    int flexDir = node->props[FLEX_DIRECTION];
    int justifyContent = node->props[JUSTIFY_CONTENT];
    int alignItems = node->props[ALIGN_ITEMS];

    float parentPrimarySize =
        flexDir == PILATES_ROW ? node->width : node->height;
    float parentSecondarySize =
        flexDir == PILATES_ROW ? node->height : node->width;

    // compute total size of children
    float childrenPrimarySize;
    updateNodeSize(flexDir, node, childrenPrimarySize);

    childrenPrimarySize = Min(parentPrimarySize, childrenPrimarySize);

    // if childrenPrimarySize is larger than node.width, we need to wrap

    // start of primary axis children box
    float primaryOffset =
        calcAxisOffset(justifyContent, childrenPrimarySize, parentPrimarySize);

    float pPos = primaryOffset;
    for (int i = 0; i < node->num_children; i++) {
      Node *child = &node->children[i];
      if (flexDir == PILATES_ROW) {
        child->x = pPos;
        pPos += child->width;
        child->y =
            calcAxisOffset(alignItems, child->height, parentSecondarySize);
      } else {
        child->y = pPos;
        pPos += child->height;
        child->x =
            calcAxisOffset(alignItems, child->width, parentSecondarySize);
      }
    }

    return;
  }
}
