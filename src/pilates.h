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
#define PILATES_SPACE_BETWEEN 3
#define PILATES_SPACE_AROUND 4
#define PILATES_SPACE_EVENLY 4

#define PILATES_MEASURE_TEXT(Name)                                             \
  void Name(int fontId, char *text, int n, float *width, float *height)
typedef PILATES_MEASURE_TEXT(MeasureTextFunc);

enum NodeType { DIV, TEXT, NodeType_COUNT };

enum PropType { FLEX_DIRECTION, JUSTIFY_CONTENT, ALIGN_ITEMS, PropType_COUNT };

struct Node {
  NodeType type;
  int props[PropType_COUNT];
  int id;
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

int strpos(char *str, char search, int offset) {
  for (int i = offset; i < strlen(str); i++) {
    if (str[i] == search) {
      return i;
    }
  }

  return -1;
}

// Currently this only supports word-wrapping mode
int computeTextLineHeight(int fontId, MeasureTextFunc *measureText, char *text,
                          float maxWidth) {
  int lines = 1;
  float lineWidth = 0;

  int n = strlen(text);
  for (int i = 0; i < n; i++) {
    int nextSpace = strpos(text, ' ', i);

    if (nextSpace < 0)
      nextSpace = n - 1;

    float width, height;
    measureText(fontId, &text[i], nextSpace - i + 1, &width, &height);
    lineWidth += width;

    if (lineWidth > maxWidth) {
      lines++;
      lineWidth = width;
    }

    i = nextSpace;
  }

  return lines;
}

void computeWidths(Node *node, MeasureTextFunc *measureText) {
}

// 1. dimension computing (fill in the blank widths)
// a. calculate what each children's dimensions would be in an ideal world
// b. add widths to things that don't have widths

// 2. dimension resolution
// based on flex prop and current width, and flex-wrap

// 3. calculate heights

// 4. layout things

void layoutNodes(Node *node, MeasureTextFunc *measureText) {
  if (node->type == TEXT) {
    return;
  }

  if (node->type == DIV) {

    return;
  }
}
