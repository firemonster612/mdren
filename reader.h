#ifndef READER_H
#define READER_H

// Reads an entire file into memory and returns a pointer to the content.
// Returns NULL if something goes wrong.
// The caller is responsible for freeing the returned pointer.
char *read_file(const char *filename);

#endif
