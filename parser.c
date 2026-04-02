#include "parser.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static void parse_hz_rule(Document *doc, char *line_start, int length) {
  if (doc->count >= doc->capacity) {
    doc->capacity *= 2;
    doc->blocks = realloc(doc->blocks, sizeof(Block) * doc->capacity);
  }
  doc->blocks[doc->count].type = BLOCK_HZ_RULE;
  doc->blocks[doc->count].heading_level = 0;

  char *text = malloc(1);
  text[0] = '\0';
  doc->blocks[doc->count].text = text;
  doc->count++;
}

static void parse_heading(Document *doc, char *line_start, int length) {
  int level;
  for (level = 0; level < 6 && line_start[level] == '#';
       level++) { // while loop prolly woulda been better
  }

  if (doc->count >= doc->capacity) {
    doc->capacity *= 2;
    doc->blocks = realloc(doc->blocks, sizeof(Block) * doc->capacity);
  }
  doc->blocks[doc->count].type = BLOCK_HEADING;
  doc->blocks[doc->count].heading_level = level;

  int text_length = length - level - 1;
  char *text = malloc(text_length + 1);
  strncpy(text, line_start + level + 1, text_length);
  text[text_length] = '\0';
  doc->blocks[doc->count].text = text;
  doc->count++;
};

static void parse_ul(Document *doc, char *line_start, int length, int indent) {
  if (doc->count >= doc->capacity) {
    doc->capacity *= 2;
    doc->blocks = realloc(doc->blocks, sizeof(Block) * doc->capacity);
  }
  doc->blocks[doc->count].type = BLOCK_UNORDERED_LIST;
  doc->blocks[doc->count].heading_level = 0;

  int text_length = length - indent - 2;
  char *text = malloc(text_length + 1);
  strncpy(text, line_start + indent + 2, text_length);
  text[text_length] = '\0';
  doc->blocks[doc->count].text = text;
  doc->count++;
}

static void parse_ol(Document *doc, char *line_start, int length, int indent) {
  if (doc->count >= doc->capacity) {
    doc->capacity *= 2;
    doc->blocks = realloc(doc->blocks, sizeof(Block) * doc->capacity);
  }
  doc->blocks[doc->count].type = BLOCK_ORDERED_LIST;
  doc->blocks[doc->count].heading_level = 0;

  int text_length = length - indent - 3;
  char *text = malloc(text_length + 1);
  strncpy(text, line_start + indent + 3, text_length);
  text[text_length] = '\0';
  doc->blocks[doc->count].text = text;
  doc->count++;
}

static void parse_bq(Document *doc, char *line_start, int length, int indent) {
  if (doc->count >= doc->capacity) {
    doc->capacity *= 2;
    doc->blocks = realloc(doc->blocks, sizeof(Block) * doc->capacity);
  }
  doc->blocks[doc->count].type = BLOCK_BLOCKQUOTE;
  doc->blocks[doc->count].heading_level = 0;

  int text_length = length - indent - 2;
  char *text = malloc(text_length + 1);
  strncpy(text, line_start + indent + 2, text_length);
  text[text_length] = '\0';
  doc->blocks[doc->count].text = text;
  doc->count++;
}

Document parse_md(char *content) {
  Document doc;
  doc.capacity = 8;
  doc.count = 0;
  doc.blocks = malloc(sizeof(Block) * doc.capacity);

  char *line_start = content;
  bool in_code_block = false;
  char *code_buf = NULL;
  int code_capacity = 0;
  int code_length = 0;

  while (line_start != NULL && *line_start != '\0') {
    char *line_end = strchr(line_start, '\n');

    int length;
    if (line_end != NULL) {
      length = line_end - line_start;
    } else {
      length = strlen(line_start);
    }

    int indent = 0;
    while (line_start[indent] == ' ' || line_start[indent] == '\t') {
      indent++;
    }

    if (in_code_block) {
      // closing backticks — save the code block
      if (line_start[indent] == '`' && line_start[indent + 1] == '`' &&
          line_start[indent + 2] == '`') {
        if (doc.count >= doc.capacity) {
          doc.capacity *= 2;
          doc.blocks = realloc(doc.blocks, sizeof(Block) * doc.capacity);
        }
        doc.blocks[doc.count].type = BLOCK_CODE;
        doc.blocks[doc.count].heading_level = 0;
        doc.blocks[doc.count].text = code_buf;
        doc.count++;
        in_code_block = false;
        code_buf = NULL;
      } else {
        // append line to code buffer
        if (code_length + length + 2 >= code_capacity) {
          code_capacity = (code_capacity + length + 2) * 2;
          code_buf = realloc(code_buf, code_capacity);
        }
        strncpy(code_buf + code_length, line_start, length);
        code_length += length;
        code_buf[code_length] = '\n';
        code_length++;
        code_buf[code_length] = '\0';
      }
    } else if (line_start[indent] == '`' && line_start[indent + 1] == '`' &&
               line_start[indent + 2] == '`') {
      // opening backticks — start collecting code
      in_code_block = true;
      code_capacity = 64;
      code_length = 0;
      code_buf = malloc(code_capacity);
      code_buf[0] = '\0';
    } else if (line_start[0] == '#') {
      parse_heading(&doc, line_start, length);
    } else if (indent <= 4 && line_start[indent] == '-' &&
               line_start[indent + 1] == ' ') {
      parse_ul(&doc, line_start, length, indent);
    } else if (indent <= 4 && line_start[indent] == '-' &&
               line_start[indent + 1] == '-' && line_start[indent + 2] == '-') {
      parse_hz_rule(&doc, line_start, length);
    } else if (indent <= 4 && isdigit(line_start[indent]) &&
               line_start[indent + 1] == '.' &&
               (line_start[indent + 2] == ' ' ||
                line_start[indent + 2] == '\t')) {
      parse_ol(&doc, line_start, length, indent);
    } else if (line_start[indent] == '>' && (line_start[indent + 1] == ' ' ||
                                             line_start[indent + 1] == '\t')) {
      parse_bq(&doc, line_start, length, indent);
    } else {
      if (doc.count >= doc.capacity) {
        doc.capacity *= 2;
        doc.blocks = realloc(doc.blocks, sizeof(Block) * doc.capacity);
      }
      doc.blocks[doc.count].type = BLOCK_PARAGRAPH;
      doc.blocks[doc.count].heading_level = 0;
      char *text = malloc(length + 1);
      strncpy(text, line_start, length);
      text[length] = '\0';
      doc.blocks[doc.count].text = text;
      doc.count++;
    }

    if (line_end != NULL) {
      line_start = line_end + 1;
    } else {
      break;
    }
  }

  return doc;
}
