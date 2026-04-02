#include "reader.h"
#include <stdio.h>
#include <stdlib.h>

char *read_file(const char *filename) {

  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    return "NULL";
  }

  fseek(fp, 0, SEEK_END);
  unsigned long size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  char *file_buf = malloc(size + 1);

  fread(file_buf, 1, size, fp);
  file_buf[size] = '\0';
  fclose(fp);
  return file_buf;
}
