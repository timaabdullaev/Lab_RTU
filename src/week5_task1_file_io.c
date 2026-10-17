#include <stdio.h>

int main(int argc, char* argv[]) {
  const char* filename = (argc > 1) ? argv[1] : "data.txt";
  char line[256];
  int count = 0;

  printf("Writing lines to %s...\n", filename);
  FILE* fp = fopen(filename, "w");
  if (fp == NULL) {
    perror("Error opening file for writing");
    return 1;
  }
  fprintf(fp, "Hello, file I/O in C!\n");
  fprintf(fp, "This is another line.\n");
  if (fclose(fp) != 0) {
    perror("Error closing file");
    return 1;
  }

  printf("Reading contents:\n");
  fp = fopen(filename, "r");
  if (fp == NULL) {
    perror("Error opening file for reading");
    return 1;
  }
  while (fgets(line, sizeof line, fp) != NULL) {
    fputs(line, stdout);
    count++;
  }
  fclose(fp);

  printf("Lines read: %d\n", count);
  return 0;
}
