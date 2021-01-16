#ifndef STR_UTILS
#define STR_UTILS

#include <string.h>

#ifdef  __cplusplus
extern "C" {
#endif

void copy_string(const char* source, char* dest, size_t from_index, size_t to_index);
int starts_with(const char* string, const char* prefix);

#ifdef  __cplusplus
}
#endif

#endif
