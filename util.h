// vim: ft=c
#ifndef UTIL_H_
#define UTIL_H_

#define ARRAY_LEN(array) (sizeof(array)/sizeof(array[0]))

#define DA_INIT_SIZE 24

static bool color = true;
static int verbose = 0;

char *shift_args(int *argc, char ***argv);
void dump_tokens(Tokens *tokens);

#define da_append(da, item)                                                     \
  do {                                                                          \
    if ((da)->count >= (da)->capacity) {                                        \
      (da)->capacity = (da)->capacity == 0 ? DA_INIT_SIZE : (da)->capacity * 2; \
      (da)->items =                                                             \
          realloc((da)->items, (da)->capacity * sizeof(*(da)->items));          \
      assert((da)->items != NULL && "Out of memory");                           \
    }                                                                           \
    (da)->items[(da)->count++] = (item);                                        \
  } while (0)

#define da_append_many(da, new_items, new_items_count)                 \
  do {                                                                 \
    if ((da)->count + new_items_count > (da)->capacity) {              \
      if ((da)->capacity == 0) {                                       \
        (da)->capacity = DA_INIT_CAP;                                  \
      }                                                                \
      while ((da)->count + new_items_count > (da)->capacity) {         \
        (da)->capacity *= 2;                                           \
      }                                                                \
      (da)->items =                                                    \
          realloc((da)->items, (da)->capacity * sizeof(*(da)->items)); \
      assert((da)->items != NULL && "Out of memory");                  \
    }                                                                  \
    memcpy((da)->items + (da)->count, new_items,                       \
           new_items_count * sizeof(*(da)->items));                    \
    (da)->count += new_items_count;                                    \
  } while (0)

#define da_free(da) free((da).items)

#endif // UTIL_H_

