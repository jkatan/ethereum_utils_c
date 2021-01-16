#include "string_utils.h"

void copy_string(const char* source, char* dest, size_t from_index, size_t to_index)
{
	size_t current_index = 0;
	while (source[current_index] != 0 && (from_index + current_index) <= to_index)
	{
		dest[from_index + current_index] = source[current_index];
		current_index++;
	}
}

int starts_with(const char* string, const char* prefix)
{
	size_t index = 0;
	while (prefix[index] != 0 && string[index] != 0)
	{
		if (prefix[index] != string[index])
		{
			return 0;
		}
		index++;
	}

	return index < strlen(prefix) ? 0 : 1;
}