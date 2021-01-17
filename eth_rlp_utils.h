#ifndef ETH_RLP_UTILS
#define ETH_RLP_UTILS

#ifdef  __cplusplus
extern "C" {
#endif

#define MAX_LENGTH_HEX_SIZE 10

char* rlp_encode_hex_string(const char* hex_string_to_encode);
char* rlp_encode_hex_string_list(const char* hex_string_list_to_encode[], unsigned int list_elements_amount);
char* rlp_add_encoding_prefix_single_byte(unsigned int encoding_constant, unsigned int number_of_bytes, const char* encoding_suffix, unsigned int encoding_suffix_length);
char* rlp_add_encoding_prefix_length_bytes(unsigned int encoding_constant, unsigned int number_of_bytes, const char* encoding_suffix, unsigned int encoding_suffix_length);

#ifdef  __cplusplus
}
#endif

#endif
