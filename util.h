#ifndef UTIL_H
#define UTIL_H

#define ARRAY_LEN(array) (sizeof((array)) / sizeof((array)[0]))

#define TRUE      -1
#define FALSE     0
#define TRUTHY(x) (((x) > FALSE) ? TRUE : FALSE)

#define DA_SIZE 24
#define da_append(da, item)                                                \
  do {                                                                     \
    if ((da)->count >= (da)->capacity) {                                   \
      (da)->capacity = (da)->capacity == 0 ? DA_SIZE : (da)->capacity * 2; \
      (da)->items =                                                        \
          realloc((da)->items, (da)->capacity * sizeof(*(da)->items));     \
      assert((da)->items != NULL && "Out of memory");                      \
    }                                                                      \
    (da)->items[(da)->count++] = (item);                                   \
  } while (0)

#define da_free(da) free((da).items)

char *read_file(const char *path);
int iswhitespace(char c);
int isnumber(char c);
void to_lower(char *s);
char *shift_args(int *argc, char ***argv);

#endif // !UTIL_H

