#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define FILENAME "students.txt"

typedef struct Student {
  char name[50];
  int id;
  float gpa;
} Student;

static int read_line(char* buf, size_t size) {
  if (fgets(buf, (int)size, stdin) == NULL) {
    return -1;
  }
  size_t len = strlen(buf);
  if (len > 0 && buf[len - 1] == '\n') {
    buf[len - 1] = '\0';
  } else {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
  }
  return 0;
}

static void trim(char* s) {
  size_t len = strlen(s);
  while (len > 0 && isspace((unsigned char)s[len - 1])) {
    s[--len] = '\0';
  }
  size_t start = 0;
  while (s[start] != '\0' && isspace((unsigned char)s[start])) {
    start++;
  }
  if (start > 0) {
    memmove(s, s + start, strlen(s + start) + 1);
  }
}

int load_students(Student arr[], int max) {
  FILE* fp = fopen(FILENAME, "r");
  if (fp == NULL) {
    return 0;
  }

  char line[256];
  int count = 0;
  while (count < max && fgets(line, sizeof line, fp) != NULL) {
    if (strchr(line, '\n') == NULL && !feof(fp)) {
      int c;
      while ((c = fgetc(fp)) != '\n' && c != EOF) {
      }
    }

    Student s = {{0}, 0, 0.0f};
    if (sscanf(line, "%49[^,],%d,%f", s.name, &s.id, &s.gpa) == 3) {
      arr[count++] = s;
    }
  }
  fclose(fp);
  return count;
}

int save_students(const Student arr[], int count) {
  FILE* fp = fopen(FILENAME, "w");
  if (fp == NULL) {
    return -1;
  }
  for (int i = 0; i < count; i++) {
    if (fprintf(fp, "%s,%d,%.2f\n", arr[i].name, arr[i].id, arr[i].gpa) < 0) {
      fclose(fp);
      return -1;
    }
  }
  if (fclose(fp) != 0) {
    return -1;
  }
  return 0;
}

void add_student(Student arr[], int* count) {
  char name[128], id_str[128], gpa_str[128];

  if (*count >= MAX_STUDENTS) {
    printf("Cannot add more students (limit reached).\n");
    return;
  }

  printf("Enter name: ");
  int ok = (read_line(name, sizeof name) == 0);
  printf("Enter ID: ");
  ok = (read_line(id_str, sizeof id_str) == 0) && ok;
  printf("Enter GPA: ");
  ok = (read_line(gpa_str, sizeof gpa_str) == 0) && ok;

  if (!ok) {
    printf("Invalid input. Student not added.\n");
    return;
  }

  trim(name);
  trim(id_str);
  trim(gpa_str);

  if (name[0] == '\0' || strchr(name, ',') != NULL ||
      strlen(name) >= sizeof arr[0].name) {
    printf("Invalid input. Student not added.\n");
    return;
  }

  char* end;
  long id = strtol(id_str, &end, 10);
  if (id_str[0] == '\0' || *end != '\0' || id < INT_MIN || id > INT_MAX) {
    printf("Invalid input. Student not added.\n");
    return;
  }

  double gpa = strtod(gpa_str, &end);
  if (gpa_str[0] == '\0' || *end != '\0' || !(gpa >= 0.0 && gpa <= 4.0)) {
    printf("Invalid input. Student not added.\n");
    return;
  }

  Student* s = &arr[*count];
  strcpy(s->name, name);
  s->id = (int)id;
  s->gpa = (float)gpa;
  (*count)++;
  printf("Student added successfully!\n");
}

void list_students(const Student arr[], int count) {
  if (count == 0) {
    printf("No students found.\n");
    return;
  }
  printf("%-5s %-20s %s\n", "ID", "Name", "GPA");
  for (int i = 0; i < count; i++) {
    printf("%-5d %-20s %.2f\n", arr[i].id, arr[i].name, arr[i].gpa);
  }
}

static int save_and_exit(const Student arr[], int count) {
  if (save_students(arr, count) != 0) {
    fprintf(stderr, "Error: could not save %s.\n", FILENAME);
    return 1;
  }
  printf("Data saved. Goodbye!\n");
  return 0;
}

int main(void) {
  Student students[MAX_STUDENTS];
  int count = load_students(students, MAX_STUDENTS);
  char buf[64];

  printf("Loaded %d student(s) from %s\n", count, FILENAME);

  for (;;) {
    printf("\n=== Student Management System ===\n");
    printf("1. List students\n");
    printf("2. Add student\n");
    printf("3. Save and Exit\n");
    printf("Select an option: ");

    if (read_line(buf, sizeof buf) != 0) {
      printf("\n");
      return save_and_exit(students, count);
    }

    trim(buf);
    if (strcmp(buf, "1") == 0) {
      list_students(students, count);
    } else if (strcmp(buf, "2") == 0) {
      add_student(students, &count);
    } else if (strcmp(buf, "3") == 0) {
      return save_and_exit(students, count);
    } else {
      printf("Invalid option. Please choose 1, 2 or 3.\n");
    }
  }
}
