#include "eth_rlp_utils.h"
#include "string_utils.h"
#include <stdio.h>
#include <stdlib.h>

char* rlp_encode_hex_string(const char* hex_string_to_encode)
{
	const size_t hex_string_length = strlen(hex_string_to_encode);
	const size_t number_of_bytes = hex_string_length / 2;

	char* encoded_string = NULL;
	if (number_of_bytes == 1 && strtol(hex_string_to_encode, NULL, 16) < 0x80)
	{
		encoded_string = malloc(3 * sizeof(char));
		if (encoded_string == NULL)
		{
			fprintf(stderr, "[eth_rlp_utils] Error allocating memory");
			return NULL;
		}
		encoded_string[0] = hex_string_to_encode[0];
		encoded_string[1] = hex_string_to_encode[1];
		encoded_string[2] = 0;
		return encoded_string;
	}

	if (number_of_bytes <= 55)
	{
		encoded_string = rlp_add_encoding_prefix_single_byte(0x80, number_of_bytes, hex_string_to_encode, hex_string_length);
	}
	else
	{
		encoded_string = rlp_add_encoding_prefix_length_bytes(0xB7, number_of_bytes, hex_string_to_encode, hex_string_length);
	}

	return encoded_string;
}
char* rlp_encode_hex_string_list(const char* hex_string_list_to_encode[], unsigned int list_elements_amount)
{
	unsigned int current_encoded_string_length = 0;
	char* encoded_concatenations = NULL;
	for (unsigned int i = 0; i < list_elements_amount; i++)
	{
		const char* list_item_to_encode = hex_string_list_to_encode[i];

		char* encoded_list_item = rlp_encode_hex_string(list_item_to_encode);
		const unsigned int encoded_list_item_length = strlen(encoded_list_item);
		if (i == 0)
		{
			encoded_concatenations = malloc(encoded_list_item_length * sizeof(char));
			if (encoded_concatenations == NULL)
			{
				fprintf(stderr, "[eth_rlp_utils] Error allocating memory");
				free(encoded_list_item);
				return NULL;
			}
		}
		else
		{
			char* tmp = realloc(encoded_concatenations, (current_encoded_string_length + encoded_list_item_length) * sizeof(char));
			if (tmp == NULL)
			{
				fprintf(stderr, "[eth_rlp_utils] Error allocating memory");
				free(encoded_list_item);
				return NULL;
			}

			encoded_concatenations = tmp;
		}
		current_encoded_string_length += encoded_list_item_length;
		copy_string(encoded_list_item, encoded_concatenations, current_encoded_string_length - encoded_list_item_length, current_encoded_string_length);
		free(encoded_list_item);
	}

	const unsigned int total_bytes = current_encoded_string_length / 2;
	char* encoded_string = NULL;
	if (total_bytes <= 55)
	{
		encoded_string = rlp_add_encoding_prefix_single_byte(0xC0, total_bytes, encoded_concatenations, current_encoded_string_length);
	}
	else
	{
		encoded_string = rlp_add_encoding_prefix_length_bytes(0xF7, total_bytes, encoded_concatenations, current_encoded_string_length);
	}

	free(encoded_concatenations);
	return encoded_string;
}

char* rlp_add_encoding_prefix_single_byte(unsigned int encoding_constant, unsigned int number_of_bytes, const char* encoding_suffix, unsigned int encoding_suffix_length)
{
	const unsigned int first_byte = encoding_constant + number_of_bytes;
	char encoding_prefix[3];
	snprintf(encoding_prefix, 3, "%02X", first_byte);

	char* encoded_string = malloc((encoding_suffix_length + 3) * sizeof(char));
	if (encoded_string == NULL)
	{
		fprintf(stderr, "[eth_rlp_utils] Error allocating memory");
		return NULL;
	}
	copy_string(encoding_prefix, encoded_string, 0, 1);
	copy_string(encoding_suffix, encoded_string, 2, encoding_suffix_length + 1);
	encoded_string[encoding_suffix_length + 2] = 0;
	return encoded_string;
}

char* rlp_add_encoding_prefix_length_bytes(unsigned int encoding_constant, unsigned int number_of_bytes, const char* encoding_suffix, unsigned int encoding_suffix_length)
{
	char number_of_bytes_hex_string[MAX_LENGTH_HEX_SIZE] = { 0 };
	snprintf(number_of_bytes_hex_string, MAX_LENGTH_HEX_SIZE, "%02X", number_of_bytes);

	const size_t number_of_bytes_hex_string_length = strlen(number_of_bytes_hex_string);
	const unsigned int first_byte = encoding_constant + (number_of_bytes_hex_string_length / 2);
	char encoding_prefix[3];
	snprintf(encoding_prefix, 3, "%02X", first_byte);

	const size_t encoded_string_length = 3 + number_of_bytes_hex_string_length + encoding_suffix_length;
	char* encoded_string = malloc(encoded_string_length * sizeof(char));
	if (encoded_string == NULL)
	{
		fprintf(stderr, "[eth_rlp_utils] Error allocating memory");
		return NULL;
	}
	copy_string(encoding_prefix, encoded_string, 0, 1);
	copy_string(number_of_bytes_hex_string, encoded_string, 2, number_of_bytes_hex_string_length + 1);
	copy_string(encoding_suffix, encoded_string, number_of_bytes_hex_string_length + 2, encoded_string_length - 2);
	encoded_string[encoded_string_length - 1] = 0;
	return encoded_string;
}