#ifndef ETH_JRPC_CLIENT
#define ETH_JRPC_CLIENT

#include "curl/curl.h"
#include "external_libs/keccak256.h"
#include "external_libs/uECC.h"

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct callback_params
{
	void (*user_callback)(char* response, void* user_data);
	void* user_callback_data;
} callback_params;

typedef struct transaction
{
	const char* nonce;
	const char* gas_price;
	const char* gas_limit;
	const char* to_address;
	const char* value;
	const char* message_call_data;
} transaction;

unsigned char* hex_string_to_hex_array(const char* hex_string, unsigned int bytes_amount);

void eth_jrpc_client_init(const char* endpoint);
void eth_jrpc_client_cleanup();
void perform_request(const char* json_data, callback_params* callback);

size_t eth_jrpc_client_callback(void* buffer, size_t size, size_t nmemb, void* userp);

void eth_blockNumber(callback_params callback);
void eth_getBlockByNumber(const char* block_number, int show_tx_details_flag, callback_params callback);
void eth_call(const char* from, const char* to, const char* function_signature, const char* function_params[], callback_params callback);
void eth_sign_and_send_transaction(const transaction* tx, unsigned int chain_id, const char* private_key);

#ifdef  __cplusplus
}
#endif

#endif