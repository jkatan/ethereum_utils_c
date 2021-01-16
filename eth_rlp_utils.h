#ifndef ETH_RLP_UTILS
#define ETH_RLP_UTILS

#ifdef  __cplusplus
extern "C" {
#endif

#define MAX_LENGTH_HEX_SIZE 10

char* rlp_encode_hex_string(const char* hex_string_to_encode);

#ifdef  __cplusplus
}
#endif

#endif
