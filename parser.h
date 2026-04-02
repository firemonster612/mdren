#ifndef PARSER_H
#define PARSER_H

typedef enum {
  BLOCK_HEADING,
  BLOCK_PARAGRAPH,
  BLOCK_UNORDERED_LIST,
  BLOCK_ORDERED_LIST,
  BLOCK_BLOCKQUOTE,
  BLOCK_CODE,
  BLOCK_HZ_RULE,
} BlockType;

typedef struct {
  BlockType type;
  int heading_level;
  char *text;
} Block;

typedef struct {
  Block *blocks;
  int count;
  int capacity;
} Document;

Document parse_md(char *content);

#endif
