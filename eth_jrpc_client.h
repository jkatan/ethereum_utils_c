#ifndef ETH_JRPC_CLIENT
#define ETH_JRPC_CLIENT

#include "curl/curl.h"

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
	char* from;
	char* to;
	unsigned int* gas;
	unsigned int* gasPrice;
	unsigned int* value;
	char* data;
} transaction_fields;

void eth_jrpc_client_init(const char* endpoint);
void eth_jrpc_client_cleanup();
void perform_request(const char* json_data, callback_params* callback);

size_t eth_jrpc_client_callback(void* buffer, size_t size, size_t nmemb, void* userp);

void eth_blockNumber(callback_params callback);
void eth_getBlockByNumber(const char* block_number, int show_tx_details_flag, callback_params callback);
void eth_call(transaction_fields tx, callback_params callback);

#ifdef  __cplusplus
}
#endif

#endif