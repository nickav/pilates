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

void printNode(Node *node, PrintFunc *printf, int indent = 0) {
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
      printNode(&node->children[i], printf, indent + 2);
    }
  }

  printf("\n");
}

PILATES_MEASURE_TEXT(asciiMeasureText) {
  *width = n ? n : strlen(text);
  *height = 1;
}

void asciiRenderNode(Node *node, char *output, int width, int height) {
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
