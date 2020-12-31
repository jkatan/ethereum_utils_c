#include "eth_abi_utils.h"
#include <string.h>
#include "keccak256.h"

char* encode_eth_call_data(const char* function_signature, const char* function_parameters[])
{
	unsigned char hash[32];
	SHA3_CTX context;
	keccak_init(&context);
	keccak_update(&context, (const unsigned char*)function_signature, (uint16_t)strlen(function_signature));
	keccak_final(&context, hash);
	memset((char*)&context, 0, sizeof(SHA3_CTX));

	unsigned char function_selector[4];
	memcpy(function_selector, hash, 4);

	char* encoded_params = encode_function_params(function_parameters, NULL);

	return NULL;
	/*
	 * char* function_signature_hash = keccak256(function_signature); - DONE
	 * char* leftmost_four_bytes = get_leftmost_four_bytes(function_signature_hash); - DONE
	 * char* encoded_params = encode_function_params(char* values[], char* types[]);
	 * char* encoded_call_data = concatenate(leftmost_four_bytes, encoded_params);
	 * return encoded_call_data;
	 */
}

char* encode_function_params(const char* values[], const char* types[])
{
	return NULL;
}
