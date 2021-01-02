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

void copy_string(const char* source, char* dest, size_t from_index, size_t to_index);

#ifdef  __cplusplus
}
#endif

#endif