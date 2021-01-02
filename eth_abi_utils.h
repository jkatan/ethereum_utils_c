#ifndef ETH_ABI
#define ETH_ABI

#ifdef  __cplusplus
extern "C" {
#endif

char* encode_eth_call_data(const char* function_signature, const char* function_parameters[]);
char* encode_function_params(const char* function_signature, const char* function_parameters[]);
void encode_function_param(const char* param_type, const char* param_value, char* output);
#ifdef  __cplusplus
}
#endif

#endif