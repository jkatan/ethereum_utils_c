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

	// Get 4 leftmost bytes of the hash
	unsigned char function_selector[4];
	memcpy(function_selector, hash, 4);

	// Encode the function parameters
	char* encoded_params = encode_function_params(function_signature, function_parameters);

	return NULL;
	/*
	 * char* function_signature_hash = keccak256(function_signature); - DONE
	 * char* leftmost_four_bytes = get_leftmost_four_bytes(function_signature_hash); - DONE
	 * char* encoded_params = encode_function_params(char* values[], char* types[]);
	 * char* encoded_call_data = concatenate(leftmost_four_bytes, encoded_params);
	 * return encoded_call_data;
	 */
}

char* encode_function_params(const char* function_signature, const char* function_parameters[])
{
	size_t index = 0;
	size_t params_read = 0;
	int first_parenthesis_read = 0;
	char* encoded_function_params;
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
			char encoded_function_param[65] = { 0 };
			encode_function_param(param_type, function_parameters[params_read], encoded_function_param);
			params_read++;
			// encoded_function_params = realloc(strlen(encoded_function_param));
			// strcat(encoded_function_params, encoded_function_param);
			// free(encoded_function_param);
		}

		if (function_signature[index] == '(')
		{
			first_parenthesis_read = 1;
		}

		index++;
	}
	return NULL;
}

void encode_function_param(const char* param_type, const char* param_value, char* output)
{
	printf("Param type: %s; Param value: %s\n", param_type, param_value);

	if (strcmp(param_type, "address") == 0)
	{
		const size_t value_length = strlen(param_value) - 2;
		const int zeros_to_fill = 64 - value_length;
		int i;
		for (i = 0; i < zeros_to_fill; i++)
		{
			output[i] = '0';
		}
		while (i - zeros_to_fill < value_length)
		{
			output[i] = param_value[i - zeros_to_fill + 2];
			i++;
		}
	}
}