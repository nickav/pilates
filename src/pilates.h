#pragma once

// We need a way to measure text
// Width: sum of the widths of all the characters
// Height: line height * # of newlines (post-wrapped)

// Every node has a width and height
// They can be provided or computed
// We want to calculate the pixel positions (upper left offset) and height/width
// Padding and margin

#ifndef __cplusplus
typedef enum { false, true } bool;
#endif

#ifdef PILATES_DEBUG

#include <stdio.h>

#define Assert(expr)                                                           \
  if (!(expr)) {                                                               \
    printf("Assert failed in function '%s' on line: %d\n", __FUNCTION__,       \
           __LINE__);                                                          \
    *(volatile int *)0 = 0;                                                    \
  }

#else

#define Assert(expr)
#define printf(...)

#endif

// Built-in Macros
#define Max(a, b) (a > b ? a : b)
#define Min(a, b) (a < b ? a : b)

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))

// Library Macros

#define PILATES_MEASURE_TEXT(Name)                                             \
  void Name(int fontId, char *text, int n, float *width, float *height)
typedef PILATES_MEASURE_TEXT(MeasureTextFunc);

#define PILATES_AUTO -1

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
#define PILATES_STRETCH 6

// flex-wrap
#define PILATES_NO_WRAP 0
#define PILATES_WRAP 1

typedef enum NodeType { DIV, TEXT, NodeType_COUNT } NodeType;

typedef enum PropType {
  FLEX_DIRECTION,
  FLEX_GROW,
  FLEX_WRAP,
  JUSTIFY_CONTENT,
  ALIGN_ITEMS,
  ALIGN_SELF,
  PropType_COUNT
} PropType;

typedef struct Node {
  NodeType type;
  int props[PropType_COUNT];
  int id;

  float x, y;
  float width, height;

  // div nodes
  struct Node *children;
  int numChildren;

  // text nodes
  char *text;
} Node;

#define ForEachChild(Parent)                                                   \
  for (int i = 0; i < Parent->numChildren; i++)                                \
    for (Node *child = &Parent->children[i]; child; child = NULL)

Node textNode(char *text) { return (Node){.type = TEXT, .text = text}; }

Node divNode() { return (Node){.type = DIV}; }

void setNodeSize(Node *node, int width, int height) {
  node->width = width;
  node->height = height;
}

void setPos1(Node *node, int dir, float x) { (&node->x)[dir] = x; }
float getPos1(Node *node, int dir) { return (&node->x)[dir]; }

void setPos2(Node *node, int dir, float y) { (&node->y)[-dir] = y; }
float getPos2(Node *node, int dir) { return (&node->y)[-dir]; }

void setSize1(Node *node, int dir, float w) { (&node->width)[dir] = w; }
float getSize1(Node *node, int dir) { return (&node->width)[dir]; }

void setSize2(Node *node, int dir, float h) { (&node->height)[-dir] = h; }
float getSize2(Node *node, int dir) { return (&node->height)[-dir]; }

#define createPropi(PropName, PropType)                                        \
  void set##PropName(Node *node, int value) { node->props[PropType] = value; } \
  int get##PropName(Node *node) { return node->props[PropType]; }

#define createPropf(PropName, PropType)                                        \
  void set##PropName(Node *node, float value) {                                \
    node->props[PropType] = value;                                             \
  }                                                                            \
  float get##PropName(Node *node) { return node->props[PropType]; }

createPropi(AlignItems, ALIGN_ITEMS);
createPropi(FlexDirection, FLEX_DIRECTION);
createPropi(JustifyContent, JUSTIFY_CONTENT);
createPropf(FlexGrow, FLEX_GROW);
createPropi(FlexWrap, FLEX_WRAP);

#undef createPropi
#undef createPropf

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

unsigned long strLength(const char *str) {
  const char *s;
  for (s = str; *s; s++)
    ;
  return (s - str);
}

int strPos(char *str, char search, int offset) {
  for (int i = offset; i < strLength(str); i++) {
    if (str[i] == search) {
      return i;
    }
  }

  return -1;
}

// TODO: fix the way that this handles breaks with newlines
int computeLetterWrapLineHeight(int fontId, MeasureTextFunc *measureText,
                                char *text, float maxWidth) {
  float width, height;
  measureText(fontId, text, 0, &width, &height);

  float wrapf = (width / maxWidth);
  int wrapi = wrapf;

  return wrapf > wrapi ? wrapi + 1 : wrapi;
}

int computeWordWrapLineHeight(int fontId, MeasureTextFunc *measureText,
                              char *text, float maxWidth) {
  int lines = 1;
  int lineStart = 0;

  int n = strLength(text);
  for (int i = 0; i < n; i++) {
    int nextSpace = strPos(text, ' ', i);

    if (nextSpace < 0)
      nextSpace = n - 1;

    float lineWidth, height;
    int charsToMeasure = nextSpace - lineStart + 1;
    measureText(fontId, &text[lineStart], charsToMeasure, &lineWidth,
                &height);

    if (lineWidth > maxWidth) {
      lines++;
      lineStart = nextSpace + 1;
    }

    i = nextSpace + 1;
  }

  return lines;
}

void computeAutoPrimarySizes(Node *node, MeasureTextFunc *measureText) {
  if (node->type == TEXT) {
    measureText(0, node->text, 0, &node->width, &node->height);
    return;
  }

  if (node->type == DIV) {
    int d = getFlexDirection(node);

    float childrenWidth = 0;
    ForEachChild(node) {
      computeAutoPrimarySizes(child, measureText);
      childrenWidth += getSize1(child, d);
    }

    // calculate auto width
    // make parent fill children (if parent doesn't have a width set)
    if (!getSize1(node, d)) {
      setSize1(node, d, childrenWidth);
    }

    return;
  }
}

void calcSecondarySizes(Node *node) {
  int d = getFlexDirection(node);

  // calculate auto height
  if (!getSize2(node, d)) {
    float maxChildrenHeight = 0;
    ForEachChild(node) {
      maxChildrenHeight = Max(maxChildrenHeight, getSize2(child, d));
    }

    setSize2(node, d, maxChildrenHeight);
  }

  int align = getAlignItems(node);

  if (align == PILATES_STRETCH) {
    ForEachChild(node) { setSize2(child, d, getSize2(node, d)); }
    return;
  }
}

void resolveFlexGrow(Node *node, int offset, int n) {
  Assert(n <= node->numChildren);
  Assert(offset <= n);

  int d = getFlexDirection(node);

  float totalFixedSize = 0;
  float totalWeight = 0;

  // calc total flex grow weight
  for (int i = offset; i < n; i++) {
    Node *child = &node->children[i];

    float grow = getFlexGrow(child);
    totalWeight += grow;
    if (grow == 0) {
      totalFixedSize += getSize1(child, d);
    }
  }

  // if the totalFixedSize is > the node.width (overflow), we need to treat
  // the child size as flex-grow
  float nodeSize1 = getSize1(node, d);
  if (totalFixedSize > nodeSize1) {
    totalFixedSize = 0;
    totalWeight = 0;

    for (int i = offset; i < n; i++) {
      Node *child = &node->children[i];
      float size1 = getSize1(child, d);
      setFlexGrow(child, size1);
      totalWeight += size1;
    }
  }

  // we're using flex grow
  if (totalWeight > 0) {
    float size = nodeSize1 - totalFixedSize;
    float pos = 0;

    for (int i = offset; i < n; i++) {
      Node *child = &node->children[i];
      float grow = getFlexGrow(child);

      if (grow > 0) {
        setSize1(child, d, size * (grow / totalWeight));
      }

      setPos1(child, d, pos);

      pos += getSize1(child, d);
    }
  }
}

void resolveSizes(Node *node, MeasureTextFunc *measureText) {
  if (getFlexWrap(node) == PILATES_NO_WRAP) {
    resolveFlexGrow(node, 0, node->numChildren);
  }

  // force text elements to wrap (to get their height)
  int d = getFlexDirection(node);
  ForEachChild(node) {
    if (child->type == TEXT) {
      int nLines = computeWordWrapLineHeight(0, measureText, child->text,
                                             getSize1(node, d));
      setSize2(child, d, nLines * getSize2(child, d));
    }
  };

  calcSecondarySizes(node);

  ForEachChild(node) resolveSizes(child, measureText);
}

void calcTotalSizeAndRows(Node *node, float *totalSize, int *totalRows,
                          float *rowHeight) {
  int d = getFlexDirection(node);
  bool wrap = getFlexWrap(node);
  float nodeWidth = getSize1(node, d);

  float outSize = 0.f;
  float outRows = 1;

  float accWidth = 0;
  float maxChildHeight = 0;
  ForEachChild(node) {
    outSize += getSize1(child, d);
    maxChildHeight = Max(maxChildHeight, getSize2(child, d));

    if (wrap) {
      accWidth += getSize1(child, d);
      bool overflow = accWidth > nodeWidth;
      if (overflow) {
        accWidth = 0;
        outRows++;
      }
    }
  }

  float nodeHeight = getSize2(node, d);
  *totalSize = outSize;
  *totalRows = outRows;
  *rowHeight = Max(maxChildHeight, nodeHeight / outRows);
}

void calcPositions(Node *node) {
  int d = getFlexDirection(node);
  int justifyContent = getJustifyContent(node);

  float nodeWidth = getSize1(node, d);
  float totalSize, rowHeight;
  int totalRows;
  calcTotalSizeAndRows(node, &totalSize, &totalRows, &rowHeight);

  float axisOffset =
      calcGroupOffset(justifyContent, totalSize, nodeWidth, node->numChildren);
  float primaryAdvance =
      calcChildSpacing(justifyContent, totalSize, nodeWidth, node->numChildren);

  bool wrap = getFlexWrap(node);
  if (wrap) {
    float rowWidth = 0;
    float accWidth = 0;
    int rowStartIndex = 0;
    int row = 0;
    float pos = axisOffset;

    ForEachChild(node) {
      accWidth += getSize1(child, d);
      bool overflow = accWidth > nodeWidth;
      if (overflow) {
        pos = axisOffset;
        accWidth = 0;
        row++;
        resolveFlexGrow(node, rowStartIndex, rowStartIndex + i);
        rowStartIndex = i;
      }

      setPos1(child, d, pos);
      setPos2(child, d, rowHeight * row);

      if (!getSize2(child, d)) {
        setSize2(child, d, rowHeight);
      }

      pos += getSize1(child, d) + primaryAdvance;
    }

    if (row < totalRows) {
      resolveFlexGrow(node, rowStartIndex, node->numChildren);
    }
  } else {
    int alignItems = getAlignItems(node);
    float pos = axisOffset;

    ForEachChild(node) {
      bool overflow = totalSize > nodeWidth;

      if (overflow) {
        setSize1(child, d, getSize1(child, d) * nodeWidth / totalSize);
      }

      setPos2(
          child, d,
          calcChildOffset(alignItems, getSize2(child, d), getSize2(node, d)));

      setPos1(child, d, pos);
      pos += getSize1(child, d) + primaryAdvance;
    }
  }

  if (node->type == DIV) {
    ForEachChild(node) calcPositions(child);
  }
}

void relativeToAbsolute(Node *node) {
  ForEachChild(node) {
    child->x += node->x;
    child->y += node->y;
    relativeToAbsolute(child);
  }
}

// Given a tree of nodes, produces the relative positions of all of them on
// screen:

// 1. primary dimension computing (fill in the auto widths)
//   a. calculate what each children's dimensions would be in an ideal world (no
//   wrapping) b. add widths to things that don't have widths

// 2. dimension resolution
// based on width, flex-grow and flex-wrap resolve what the width and height
// of the node should be
//   a. calculate heights
//   based on text wrapping and aspect-ratio, compute the height of the elements

// 3. find positions
// layout the nodes (giving relative positions to their parents) based on
// spacing and wrapping
void layoutNodes(Node *node, MeasureTextFunc *measureText) {
  computeAutoPrimarySizes(node, measureText);
  resolveSizes(node, measureText);
  calcPositions(node);
  relativeToAbsolute(node);
}
