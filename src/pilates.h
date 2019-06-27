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

#define PILATES_MEASURE_TEXT(Name)                                             \
  void Name(int fontId, char *text, int n, float *width, float *height)
typedef PILATES_MEASURE_TEXT(MeasureTextFunc);

// flex-direction
#define PILATES_ROW 0
#define PILATES_COLUMN 1

// both for justify-content and align-items
#define PILATES_ALIGN_START 0
#define PILATES_ALIGN_CENTER 1
#define PILATES_ALIGN_END 2
#define PILATES_SPACE_BETWEEN 3
#define PILATES_SPACE_AROUND 4
#define PILATES_SPACE_EVENLY 5

#define PILATES_NO_WRAP 0
//#define PILATES_WRAP 1

enum NodeType { DIV, TEXT, NodeType_COUNT };

enum PropType {
  FLEX_DIRECTION,
  FLEX_WRAP,
  JUSTIFY_CONTENT,
  ALIGN_ITEMS,
  ALIGN_SELF,
  PropType_COUNT
};

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

void setPos1(Node *node, int dir, float x) { (&node->x)[dir] = x; }
float getPos1(Node *node, int dir) { return (&node->x)[dir]; }

void setPos2(Node *node, int dir, float y) { (&node->y)[-dir] = y; }
float getPos2(Node *node, int dir) { return (&node->y)[-dir]; }

void setSize1(Node *node, int dir, float width) { (&node->width)[dir] = width; }
float getSize1(Node *node, int dir) { return (&node->width)[dir]; }

void setSize2(Node *node, int dir, float height) {
  (&node->height)[-dir] = height;
}
float getSize2(Node *node, int dir) { return (&node->height)[-dir]; }

#define createPropSetter(PropName, PropType)                                   \
  void PropName(Node *node, int value) { node->props[PropType] = value; }

createPropSetter(setAlignItems, ALIGN_ITEMS);
createPropSetter(setFlexDirection, FLEX_DIRECTION);
createPropSetter(setJustifyContent, JUSTIFY_CONTENT);

#undef createPropSetter

float calcChildOffset(int value, float size, float parentSize) {
  // don't allow overflow
  size = Min(parentSize, size);

  switch (value) {
  case PILATES_ALIGN_CENTER:
    return (parentSize - size) / 2;
  case PILATES_ALIGN_END:
    return parentSize - size;
  case PILATES_ALIGN_START:
  default:
    return 0;
  }
}

float calcGroupOffset(int value, float size, float parentSize, int n) {
  // don't allow overflow
  size = Min(parentSize, size);

  switch (value) {
    // alignment
  case PILATES_ALIGN_START: {
    return 0;
  }
  case PILATES_ALIGN_CENTER: {
    return (parentSize - size) / 2;
  }
  case PILATES_ALIGN_END: {
    return parentSize - size;
  }

    // spacing
  case PILATES_SPACE_EVENLY: {
    return (parentSize - size) / (n + 1);
  }
  case PILATES_SPACE_AROUND: {
    return (parentSize - size) / (2 * n);
  }
  case PILATES_SPACE_BETWEEN: {
    return 0;
  }

  default: {
    return 0;
  }
  }
}

float calcChildSpacing(int value, float size, float parentSize, int n) {
  size = Min(parentSize, size);

  switch (value) {
  case PILATES_SPACE_EVENLY: {
    return (parentSize - size) / (n + 1);
  }
  case PILATES_SPACE_AROUND: {
    return (parentSize - size) / n;
  }
  case PILATES_SPACE_BETWEEN: {
    return (parentSize - size) / (n - 1);
  }
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

void computeWidths(Node *node, MeasureTextFunc *measureText) {}

void computePrimarySize(Node *node, MeasureTextFunc *measureText) {
  if (node->type == TEXT) {
    measureText(0, node->text, 0, &node->width, &node->height);
    return;
  }

  if (node->type == DIV) {
    return;
  }
}

void resolvePrimarySize(Node *node) {
  if (node->type == TEXT) {
    return;
  }

  if (node->type == DIV) {
    return;
  }
}

// 1. dimension computing (fill in the blank widths)
// a. calculate what each children's dimensions would be in an ideal world
// b. add widths to things that don't have widths

// 2. dimension resolution
// based on flex prop and current width, and flex-wrap

// 3. calculate heights

// 4. find positions
void calcSecondarySizes(Node *node) {}

void calcPositions(Node *node) {
  // flex-wrap: no-wrap;

  int d = node->props[FLEX_DIRECTION];

  float totalSize = 0.f;
  ForEachChild(node, { totalSize += getSize1(child, d); });

  float axisOffset = calcGroupOffset(node->props[JUSTIFY_CONTENT], totalSize,
                                     getSize1(node, d), node->num_children);
  float primaryAdvance =
      calcChildSpacing(node->props[JUSTIFY_CONTENT], totalSize,
                       getSize1(node, d), node->num_children);

  float prevPos = axisOffset;
  ForEachChild(node, {
    float nodeWidth = getSize1(node, d);
    if (totalSize > nodeWidth) {
      setSize1(child, d, getSize1(child, d) * nodeWidth / totalSize);
    }

    setPos2(child, d,
            calcChildOffset(node->props[ALIGN_ITEMS], getSize2(child, d),
                            getSize2(node, d)));
    setPos1(child, d, prevPos);
    prevPos += getSize1(child, d) + primaryAdvance;
  });

  if (node->type == DIV) {
    ForEachChild(node, { calcPositions(child); });
  }
}

void convertToAbsolute(Node *node) {
  ForEachChild(node, {
    child->x += node->x;
    child->y += node->y;

    convertToAbsolute(child);
  });
}

void layoutNodes(Node *node, MeasureTextFunc *measureText) {
  computePrimarySize(node, measureText);

  resolvePrimarySize(node);

  calcSecondarySizes(node);

  calcPositions(node);

  convertToAbsolute(node);
}
