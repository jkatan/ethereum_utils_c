#ifndef ETH_ABI
#define ETH_ABI

#ifdef  __cplusplus
extern "C" {
#endif

#define MIN_BLOCK_SIZE 65
#define PARAM_TYPE_MAX_LENGTH 10
	
char* encode_eth_call_data(const char* function_signature, const char* function_parameters[]);
char* encode_function_params(const char* function_signature, const char* function_parameters[]);
char* encode_function_param(const char* param_type, const char* param_value);
char* encode_address_type(const char* value_to_encode);
char* encode_uint_type(const char* value_to_encode);

void pad_encoding_zeros(const char* value_to_encode, size_t value_length, char* output);
void copy_string(const char* source, char* dest, size_t from_index, size_t to_index);
int starts_with(const char* string, const char* prefix);

#ifdef  __cplusplus
}
#endif

#endif