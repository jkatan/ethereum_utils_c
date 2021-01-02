#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "eth_abi_utils.h"
#include "keccak256.h"

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
		snprintf(function_selector + i*2 + 2, 3, "%x", hash[i]);
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
	copy_string(encoded_params, encoded_call_data, 10, 10 + encoded_params_length);
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
			char param_type[10] = {0};
			size_t delimited_str_index = 0;
			while (function_signature[index] != ',' && function_signature[index] != ')')
			{
				param_type[delimited_str_index] = function_signature[index];
				index++;
				delimited_str_index++;
			}
			char* encoded_function_param = encode_function_param(param_type, function_parameters[params_read]);
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
	char* encoded_result = calloc(MIN_BLOCK_SIZE, sizeof(char));
	if (encoded_result == NULL)
	{
		fprintf(stderr, "[eth_abi_utils] Error allocating memory");
		return NULL;
	}
	if (strcmp(param_type, "address") == 0)
	{
		const size_t value_length = strlen(param_value) - 2;
		const size_t zeros_to_fill = MIN_BLOCK_SIZE - value_length - 1;
		int i;
		for (i = 0; i < zeros_to_fill; i++)
		{
			encoded_result[i] = '0';
		}
		while (i - zeros_to_fill < value_length)
		{
			encoded_result[i] = param_value[i - zeros_to_fill + 2];
			i++;
		}
	}
	return encoded_result;
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