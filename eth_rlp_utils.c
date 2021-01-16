#include "eth_rlp_utils.h"
#include "string_utils.h"
#include <stdio.h>
#include <stdlib.h>

char* rlp_encode_hex_string(const char* hex_string_to_encode)
{
	const size_t hex_string_length = strlen(hex_string_to_encode);
	const size_t number_of_bytes = hex_string_length / 2;

	if (number_of_bytes == 1 && strtol(hex_string_to_encode, NULL, 16) < 0x80)
	{
		char* encoded_string = malloc(3 * sizeof(char));
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
		const unsigned int first_byte = 0x80 + number_of_bytes;
		char encoding_prefix[3];
		snprintf(encoding_prefix, 3, "%02x", first_byte);

		char* encoded_string = malloc((hex_string_length + 3) * sizeof(char));
		if (encoded_string == NULL)
		{
			fprintf(stderr, "[eth_rlp_utils] Error allocating memory");
			return NULL;
		}
		copy_string(encoding_prefix, encoded_string, 0, 1);
		copy_string(hex_string_to_encode, encoded_string, 2, hex_string_length + 1);
		encoded_string[hex_string_length + 2] = 0;
		return encoded_string;
	}

	char number_of_bytes_hex_string[MAX_LENGTH_HEX_SIZE] = { 0 };
	snprintf(number_of_bytes_hex_string, MAX_LENGTH_HEX_SIZE, "%02x", number_of_bytes);

	const size_t number_of_bytes_hex_string_length = strlen(number_of_bytes_hex_string);
	const unsigned int first_byte = 0xb7 + (number_of_bytes_hex_string_length / 2);
	char encoding_prefix[3];
	snprintf(encoding_prefix, 3, "%02x", first_byte);

	const size_t encoded_string_length = 3 + number_of_bytes_hex_string_length + hex_string_length;
	char* encoded_string = malloc(encoded_string_length * sizeof(char));
	if (encoded_string == NULL)
	{
		fprintf(stderr, "[eth_rlp_utils] Error allocating memory");
		return NULL;
	}
	copy_string(encoding_prefix, encoded_string, 0, 1);
	copy_string(number_of_bytes_hex_string, encoded_string, 2, number_of_bytes_hex_string_length + 1);
	copy_string(hex_string_to_encode, encoded_string, number_of_bytes_hex_string_length + 2, encoded_string_length - 2);
	encoded_string[encoded_string_length - 1] = 0;
	return encoded_string;
}
