#include "parser.h"
#include "reader.h"
#include "renderer.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: mdren <filename>\n");
    return 1;
  }

  char *content = read_file(argv[1]);
  if (content == NULL) {
    printf("Error: could not read file '%s'\n", argv[1]);
    return 1;
  }

  Document doc = parse_md(content);
  render(doc);

  for (int i = 0; i < doc.count; i++) {
    free(doc.blocks[i].text);
  }
  free(doc.blocks);
  free(content);
  return 0;
}
