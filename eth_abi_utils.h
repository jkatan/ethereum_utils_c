#ifndef ETH_ABI_UTILS
#define ETH_ABI_UTILS

#ifdef  __cplusplus
extern "C" {
#endif

#define MIN_BLOCK_SIZE 65
#define PARAM_TYPE_MAX_LENGTH 10
#define FUNCTION_SELECTOR_LENGTH 10
	
char* encode_eth_call_data(const char* function_signature, const char* function_parameters[]);
char* encode_function_params(const char* function_signature, const char* function_parameters[]);
char* encode_function_param(const char* param_type, const char* param_value);
char* encode_address_type(const char* value_to_encode);
char* encode_uint_type(const char* value_to_encode);

void pad_encoding_zeros(const char* value_to_encode, size_t value_length, char* output);

#ifdef  __cplusplus
}
#endif

#endif
