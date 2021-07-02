#include "utils.h"
namespace split {
// constexpr auto bufsize=2056;
char **split_msg(char *buf, const char schar, size_t *splitted_size,
                 size_t msg_len) {
  /*
   */
  const unsigned long long max_splitted = 120;
  size_t arr_size = 0;
  char **splitted;
  splitted = (char **)malloc(sizeof(char *) * arr_size);
  if (!splitted)
    abort();
  char *str;
  char str2[msg_len];
  do {
    bzero(str2, msg_len);
    str = strchr(buf, schar);

    if (str != NULL || (str = strchr(buf, '\r')) != NULL) {
      arr_size++;
      splitted = (char **)realloc(splitted, sizeof(char *) * arr_size);
      if (!splitted)
        abort();

      memcpy(str2, buf, (str - buf));
      // printf("str2 = %s\n", str2);

      splitted[arr_size - 1] = (char *)malloc(sizeof(char) * strlen(str2) + 1);
      if (!splitted[arr_size - 1])
        abort();

      strcpy(splitted[arr_size - 1], str2);
      splitted[arr_size - 1][strlen(str2)] = 0;
    }
    buf = str + 1;
  } while (str != NULL && arr_size < max_splitted);
  *splitted_size = arr_size;

  return splitted;
}
void free_splitted(char **what, size_t n) {
  for (n; n--;) {
    if (n == 0)
      break;
    if (what[n][0] == 0)
      continue;
    free((void *)(what[n - 1]));
  }
  free((void *)what);
}

std::vector<std::string> split(char *buf, const char schar, size_t bufsize) {
  size_t split_size;
  std::vector<std::string> rt;
  if (bufsize == 0)
    bufsize = strlen(buf);
  char **splitted = split_msg(buf, schar, &split_size, bufsize);
  for (size_t i = 0; i < split_size; i++) {
    rt.push_back(splitted[i]);
  }
  free_splitted(splitted, split_size);
  return rt;
}

}; // namespace split
