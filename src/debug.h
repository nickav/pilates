#pragma once

#include <stdlib.h>
#include <string.h>

#ifndef PILATES_PRINT_FUNC
#include <stdio.h>
#define PILATES_PRINT_FUNC printf
#endif

#define Assert(expr)                                                           \
  if (!(expr)) {                                                               \
    PILATES_PRINT_FUNC("Assert failed in function '%s' on line: %d\n",         \
                       __FUNCTION__, __LINE__);                                \
    *(volatile int *)0 = 0;                                                    \
  }

#define PILATES_PRINT(name) int name(const char *fmt, ...);
typedef PILATES_PRINT(PrintFunc);

void printNode(Node *node, bool verbose, PrintFunc *printf, int indent = 0) {
  static char indent_str[64];

  int i;
  for (i = 0; i < indent; i++) {
    indent_str[i] = ' ';
  }
  indent_str[i + 1] = '\0';

  printf("%s", indent_str);

  char *name = (char *)(node->type == TEXT ? "TextNode" : "DivNode");

  // print name position and size
  printf("%s #%d: (%.2f, %.2f, %.2f, %.2f)", name, node->id, node->x, node->y,
         node->width, node->height);

  // print flex props
  if (verbose) {
    printf("\n");
    printf("  %sflex-grow: %.2f;\n", indent_str, getFlexGrow(node));
    printf("  %sjustify-content: %.2f;\n", indent_str, getJustifyContent(node));
    printf("  %salign-items: %d;\n", indent_str, getAlignItems(node));
    printf("  %sflex-wrap: %d;\n", indent_str, getFlexWrap(node));
    printf("\n");
  }

  if (node->num_children > 0) {
    for (int i = 0; i < node->num_children; i++) {
      printNode(&node->children[i], verbose, printf, indent + 2);
    }
  }
}

PILATES_MEASURE_TEXT(asciiMeasureText) {
  *width = n ? n : strlen(text);
  *height = 1;
}

// TODO: bounds check on all parts of the node (x, y, width, height)
void asciiRenderNode(Node *node, char *output, char *colorOutput, int width,
                     int height) {
  // we have the node's position

  if (node->type == TEXT) {
    int textLen = strlen(node->text);

    for (int i = 0; i < node->height; i++) {
      int index = (i + node->y) * width + node->x;
      int textOffset = node->width * i;

      // keep text within node bounds and textLen
      int nChars = Min(Min(width, node->width), textLen - textOffset);

      // don't let text overflow buffer
      nChars = Min(nChars, width * height - index);

      if (nChars > 0) {
        memcpy(&output[index], &node->text[textOffset], nChars);
      }
    }

  } else {
    // copy width * height of the corresponding colors item to the colorOutput
    // at index 0
    for (int x = node->x; x < node->x + node->width; x++) {
      for (int y = node->y; y < node->y + node->height; y++) {
        colorOutput[y * width + x] = node->id;
      }
    }

    ForEachChild(
        node, { asciiRenderNode(child, output, colorOutput, width, height); });
  }
}

const char *colors[] = {
    "\033[0m",    // none
    "\033[1;41m", // red
    "\033[1;42m", // green
    "\033[1;43m", // yellow
    "\033[1;44m", // blue
    "\033[1;45m", // ??
    "\033[1;46m", // ??
    "\033[1;47m", // ??
    "\033[1;48m", // ??
};

void asciiRender(Node *node) {
  int width = (int)node->width;
  int height = (int)node->height;

  // alloc temporary storage
  int buf_len = width * height + 1;
  char *buffer = (char *)malloc(sizeof(char) * (buf_len));
  memset(buffer, ' ', buf_len);
  buffer[buf_len] = '\0';
  char *colorBuf = (char *)malloc(sizeof(char) * (buf_len));

  asciiRenderNode(node, buffer, colorBuf, width, height);

  // print the grid
  for (int i = 0; i < width * 2; i++)
    printf("-");
  printf("\n");

  char prevColor = 0;
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      int i = width * y + x;

      char currColor = colorBuf[i];
      if (prevColor != currColor) {
        printf("%s", colors[colorBuf[i]]);
        prevColor = currColor;
      }

      // because ascii chars have about a 1:2 ratio, stretch by x:
      putchar(buffer[i]);
      putchar(buffer[i]);
    }

    printf("%s\n", colors[0]);
    prevColor = 0;
  }

  for (int i = 0; i < width * 2; i++)
    printf("-");

  free(buffer);
  free(colorBuf);
}
