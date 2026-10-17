#include <stdio.h>

typedef struct Student {
  char name[50];
  int age;
  float gpa;
} Student;

int save_student(const Student* s, const char* filename) {
  FILE* fp = fopen(filename, "w");
  if (fp == NULL) {
    return -1;
  }
  if (fprintf(fp, "%s %d %.2f\n", s->name, s->age, s->gpa) < 0) {
    fclose(fp);
    return -1;
  }
  if (fclose(fp) != 0) {
    return -1;
  }
  return 0;
}

int load_student(const char* filename, Student* out) {
  FILE* fp = fopen(filename, "r");
  if (fp == NULL) {
    return -1;
  }
  Student tmp = {{0}, 0, 0.0f};
  /* %49s leaves room for the terminating '\0' in name[50] */
  int n = fscanf(fp, "%49s %d %f", tmp.name, &tmp.age, &tmp.gpa);
  fclose(fp);
  if (n != 3) {
    return -1;
  }
  *out = tmp;
  return 0;
}

int main(void) {
  const char* filename = "student.txt";
  Student original = {"Alice", 21, 3.75f};
  Student loaded = {{0}, 0, 0.0f};

  printf("Saving student to file...\n");
  if (save_student(&original, filename) != 0) {
    fprintf(stderr, "Error: could not save student.\n");
    return 1;
  }

  printf("Loading student from file...\n");
  if (load_student(filename, &loaded) != 0) {
    fprintf(stderr, "Error: could not load student.\n");
    return 1;
  }

  printf("Loaded student: %s, %d, GPA %.2f\n", loaded.name, loaded.age,
         loaded.gpa);
  return 0;
}
