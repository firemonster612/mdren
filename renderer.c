#include "renderer.h"
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

// Escape codes
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define DIM "\033[2m"
#define ITALIC "\033[3m"
#define UNDERLINE "\033[4m"

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define GRAY "\033[90m"
#define ORANGE "\033[38;5;208m"

// Background colors
// Dark backgrounds (standard)
#define BG_BLACK "\033[40m"
#define BG_RED "\033[41m"
#define BG_GREEN "\033[42m"
#define BG_YELLOW "\033[43m"
#define BG_BLUE "\033[44m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN "\033[46m"
#define BG_WHITE "\033[47m"

// Bright backgrounds
#define BG_GRAY "\033[100m"
#define BG_BRIGHT_RED "\033[101m"
#define BG_BRIGHT_GREEN "\033[102m"
#define BG_BRIGHT_YELLOW "\033[103m"
#define BG_BRIGHT_BLUE "\033[104m"
#define BG_BRIGHT_MAGENTA "\033[105m"
#define BG_BRIGHT_CYAN "\033[106m"
#define BG_BRIGHT_WHITE "\033[107m"

// Bright foreground colors
#define BRIGHT_RED "\033[91m"
#define BRIGHT_GREEN "\033[92m"
#define BRIGHT_YELLOW "\033[93m"
#define BRIGHT_BLUE "\033[94m"
#define BRIGHT_MAGENTA "\033[95m"
#define BRIGHT_CYAN "\033[96m"
#define BRIGHT_WHITE "\033[97m"

// Drawing characters
#define BQ_BAR "▋"
#define HR_LINE "\u2500" // ─ horizontal line

// === Escape code reference ===
// Reset:       \033[0m
//
// Style:
//   Bold:      \033[1m
//   Dim:       \033[2m
//   Italic:    \033[3m
//   Underline: \033[4m
//   Strikethrough: \033[9m
//
// Foreground colors:
//   Black:     \033[30m
//   Red:       \033[31m
//   Green:     \033[32m
//   Yellow:    \033[33m
//   Blue:      \033[34m
//   Magenta:   \033[35m
//   Cyan:      \033[36m
//   White:     \033[37m
//
// Bright foreground:
//   Bright black (gray): \033[90m
//   Bright red:    \033[91m
//   Bright green:  \033[92m
//   Bright yellow: \033[93m
//   Bright blue:   \033[94m
//   Bright magenta:\033[95m
//   Bright cyan:   \033[96m
//   Bright white:  \033[97m
//
// Background colors:
//   Same as foreground but 40-47 and 100-107
//
// Combine them: \033[1;36m = bold + cyan

void render(Document doc) {
  int ol_num = 0;
  for (int i = 0; i < doc.count; i++) {
    Block block = doc.blocks[i];
    if (block.type != BLOCK_ORDERED_LIST) {
      ol_num = 0;
    }

    switch (block.type) {
    case BLOCK_HEADING: {
      const char *color;
      switch (block.heading_level) {
      case 1:
        color = BOLD BLUE;
        break;
      case 2:
        color = BOLD YELLOW;
        break;
      case 3:
        color = BOLD GREEN;
        break;
      case 4:
        color = BOLD CYAN;
        break;
      case 5:
        color = BOLD MAGENTA;
        break;
      case 6:
        color = BOLD BRIGHT_MAGENTA;
        break;
      default:
        color = BOLD;
        break;
      }
      printf("%s", color);
      for (int j = 0; j < block.heading_level; j++) {
        printf("#");
      }
      printf(" %s" RESET "\n", block.text);
      break;
    }
    case BLOCK_PARAGRAPH:
      printf("%s\n", block.text);
      break;
    case BLOCK_UNORDERED_LIST:
      printf(ORANGE "● " RESET "%s\n", block.text);
      break;
    case BLOCK_ORDERED_LIST: {
      ol_num++;
      printf(YELLOW "%d. " RESET "%s\n", ol_num, block.text);
      break;
    }
    case BLOCK_BLOCKQUOTE:
      printf(DIM GRAY "%s " RESET DIM "%s" RESET "\n", BQ_BAR, block.text);
      break;
    case BLOCK_CODE:
      printf(GREEN "%s" RESET, block.text);
      break;
    case BLOCK_HZ_RULE: {
      struct winsize w;
      ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
      int width = w.ws_col;
      printf(DIM);
      for (int j = 0; j < width; j++) {
        printf("%s", HR_LINE);
      }
      printf(RESET "\n");
      break;
    }
    }
  }
}
