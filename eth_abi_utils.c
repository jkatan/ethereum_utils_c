#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "eth_abi_utils.h"
#include "external_libs/keccak256.h"

char* encode_eth_call_data(const char* function_signature, const char* function_parameters[])
{
	// Get keccak256 hash of the function_signature
	unsigned char hash[32];
	SHA3_CTX context;
	keccak_init(&context);
	keccak_update(&context, (const unsigned char*)function_signature, (uint16_t)strlen(function_signature));
	keccak_final(&context, hash);
	memset((char*)&context, 0, sizeof(SHA3_CTX));

	// Store 4 leftmost bytes of the hash in a string
	char function_selector[11] = { '0', 'x' };
	for (int i = 0; i < 4; ++i)
	{
		snprintf(function_selector + i*2 + 2, 3, "%02x", hash[i]);
	}

	// Encode the function parameters
	char* encoded_params = encode_function_params(function_signature, function_parameters);
	const size_t encoded_params_length = strlen(encoded_params);

	char* encoded_call_data = malloc((10 + encoded_params_length + 1) * sizeof(char));
	if (encoded_call_data == NULL)
	{
		fprintf(stderr, "[eth_abi_utils] Error allocating memory");
		free(encoded_params);
		return NULL;
	}

	// Concatenate the function selector with the parameters encoding
	copy_string(function_selector, encoded_call_data, 0, 9);
	copy_string(encoded_params, encoded_call_data, 10, 9 + encoded_params_length);
	encoded_call_data[10 + encoded_params_length] = 0;

	free(encoded_params);

	return encoded_call_data;
}

char* encode_function_params(const char* function_signature, const char* function_parameters[])
{
	size_t index = 0;
	size_t params_read = 0;
	size_t encoded_function_params_length = 0;
	int first_parenthesis_read = 0;
	char* encoded_function_params = NULL;
	while (function_signature[index] != 0)
	{
		if (first_parenthesis_read)
		{
			char param_type[PARAM_TYPE_MAX_LENGTH] = { 0 };
			size_t delimited_str_index = 0;
			while (function_signature[index] != ',' && function_signature[index] != ')')
			{
				param_type[delimited_str_index] = function_signature[index];
				index++;
				delimited_str_index++;
			}
			char* encoded_function_param = encode_function_param(param_type, function_parameters[params_read]);
			if (encoded_function_param == NULL)
			{
				return NULL;
			}
			size_t new_block_size = strlen(encoded_function_param);
			params_read++;
			if (function_signature[index] == ')')
			{
				new_block_size += 1;
			}
			if (params_read == 1)
			{
				encoded_function_params = malloc((encoded_function_params_length + new_block_size) * sizeof(char));
				if (encoded_function_params == NULL)
				{
					fprintf(stderr, "[eth_abi_utils] Error allocating memory");
					free(encoded_function_param);
					return NULL;
				}
			}
			else
			{
				char* tmp = realloc(encoded_function_params, (encoded_function_params_length + new_block_size) * sizeof(char));
				if (tmp == NULL)
				{
					fprintf(stderr, "[eth_abi_utils] Error allocating memory");
					free(encoded_function_param);
					return NULL;
				}

				encoded_function_params = tmp;
			}
			encoded_function_params_length += new_block_size;
			copy_string(encoded_function_param, encoded_function_params, encoded_function_params_length - new_block_size, encoded_function_params_length);
			free(encoded_function_param);
			if (function_signature[index] == ')')
			{
				encoded_function_params[encoded_function_params_length - 1] = 0;
			}
		}

		if (function_signature[index] == '(')
		{
			first_parenthesis_read = 1;
		}

		index++;
	}
	return encoded_function_params;
}

char* encode_function_param(const char* param_type, const char* param_value)
{
	if (strcmp(param_type, "address") == 0)
	{
		return encode_address_type(param_value);
	}

	if (starts_with(param_type, "uint"))
	{
		return encode_uint_type(param_value);
	}

	if (strcmp(param_type, "bool") == 0)
	{
		if (strcmp(param_value, "true") == 0)
		{
			return encode_uint_type("1");
		}

		if (strcmp(param_value, "false") == 0)
		{
			return encode_uint_type("0");
		}
	}

	fprintf(stderr, "[eth_abi_utils] Invalid parameter type");
	return NULL;
}

char* encode_address_type(const char* value_to_encode)
{
	char* encoded_result = calloc(MIN_BLOCK_SIZE, sizeof(char));
	if (encoded_result == NULL)
	{
		fprintf(stderr, "[eth_abi_utils] Error allocating memory");
		return NULL;
	}

	pad_encoding_zeros(value_to_encode + 2, strlen(value_to_encode) - 2, encoded_result);

	return encoded_result;
}

char* encode_uint_type(const char* value_to_encode)
{
	char* encoded_result = calloc(MIN_BLOCK_SIZE, sizeof(char));
	if (encoded_result == NULL)
	{
		fprintf(stderr, "[eth_abi_utils] Error allocating memory");
		return NULL;
	}

	char value_to_hex[MIN_BLOCK_SIZE] = { 0 };
	int value = atoi(value_to_encode);
	snprintf(value_to_hex, MIN_BLOCK_SIZE, "%02x", value);

	pad_encoding_zeros(value_to_hex, strlen(value_to_hex), encoded_result);

	return encoded_result;
}

void pad_encoding_zeros(const char* value_to_encode, size_t value_length, char* output)
{
	const size_t zeros_to_fill = MIN_BLOCK_SIZE - value_length - 1;
	size_t i;
	for (i = 0; i < zeros_to_fill; i++)
	{
		output[i] = '0';
	}
	while (i - zeros_to_fill < value_length)
	{
		output[i] = value_to_encode[i - zeros_to_fill];
		i++;
	}
}

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
