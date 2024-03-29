#include "eth_jrpc_client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eth_abi_utils.h"
#include "eth_rlp_utils.h"
#include "external_libs/keccak256.h"
#include "external_libs/uECC.h"

const char* provider_endpoint;

void eth_jrpc_client_init(const char* endpoint)
{
	provider_endpoint = endpoint;
	curl_global_init(CURL_GLOBAL_ALL);
}

void eth_jrpc_client_cleanup()
{
	curl_global_cleanup();
}

size_t eth_jrpc_client_callback(void* buffer, size_t size, size_t nmemb, void* userp)
{
    const size_t bytes_received = size * nmemb;
    callback_params* callback_data = (callback_params*)userp;

    char* response = malloc((bytes_received + 1) * sizeof(char));
	if (response == NULL)
	{
        fprintf(stderr, "[eth_jrpc_client] Error allocating memory for the response of the request");
        return 0;
	}

    memcpy(response, buffer, bytes_received);
    response[bytes_received] = 0;
    callback_data->user_callback(response, callback_data->user_callback_data);
    free(response);

    return size * nmemb;
}

void perform_request(const char* json_data, callback_params* callback)
{
    CURL* handle = curl_easy_init();
    CURLcode response;
    struct curl_slist* headers = NULL;

    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(handle, CURLOPT_URL, provider_endpoint);
    curl_easy_setopt(handle, CURLOPT_POSTFIELDS, json_data);
    curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, eth_jrpc_client_callback);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, callback);

    response = curl_easy_perform(handle);

    curl_slist_free_all(headers);

    if (response != CURLE_OK)
    {
        fprintf(stderr, "[eth_jrpc_client] Error processing request");
    }

    curl_easy_cleanup(handle);
}

void eth_blockNumber(callback_params callback)
{

    const char* json_object = "{\"jsonrpc\":\"2.0\",\"method\":\"eth_blockNumber\",\"params\":[],\"id\":1}";
    perform_request(json_object, &callback);
}
void eth_getBlockByNumber(const char* block_number, int show_tx_details_flag, callback_params callback)
{
    const char* format = "{\"jsonrpc\":\"2.0\",\"method\":\"eth_getBlockByNumber\",\"params\":[\"%s\",%s],\"id\":1}";
    const char* details_flag = show_tx_details_flag != 0 ? "true" : "false";
    const size_t json_object_length = strlen(format) + strlen(block_number) + strlen(details_flag) - 3;

    char* json_object = calloc(json_object_length, sizeof(char));
    if (json_object == NULL)
    {
        fprintf(stderr, "[eth_jrpc_client] Error allocating memory for the request");
        return;
    }

    snprintf(json_object, json_object_length, format, block_number, details_flag);
    perform_request(json_object, &callback);
	
    free(json_object);
}

void eth_call(const char* from, const char* to, const char* function_signature, const char* function_params[], callback_params callback)
{
    char* encoded_call_data = encode_eth_call_data(function_signature, function_params);
    const char* format = "{\"jsonrpc\":\"2.0\",\"method\":\"eth_call\",\"params\":[{\"from\":\"%s\",\"to\":\"%s\",\"data\":\"%s\"}, \"latest\"],\"id\":1}";
    const size_t json_object_length = strlen(format) + strlen(from) + strlen(to) + strlen(encoded_call_data);

    char* json_object = calloc(json_object_length, sizeof(char));
    if (json_object == NULL)
    {
        fprintf(stderr, "[eth_jrpc_client] Error allocating memory for the request");
        return;
    }

    snprintf(json_object, json_object_length, format, from, to, encoded_call_data);
    perform_request(json_object, &callback);

    free(encoded_call_data);
    free(json_object);
}

void eth_sign_and_send_transaction(const transaction* tx, unsigned int chain_id, const char* private_key)
{
    char chain_id_hex[3];
    snprintf(chain_id_hex, 3, "%02X", chain_id);

    const char* fields_to_encode[] = { tx->nonce, tx->gas_price, tx->gas_limit, tx->to_address, tx->value, tx->message_call_data, chain_id_hex, "", "" };
    char* encoded_string = rlp_encode_hex_string_list(fields_to_encode, 9);
    printf("Encoded string: %s\n", encoded_string);

    unsigned int bytes_amount = strlen(encoded_string) / 2;
    unsigned char* encoded_hex_array = hex_string_to_hex_array(encoded_string, bytes_amount);
	if (encoded_hex_array == NULL)
	{
        free(encoded_string);
        return;
	}

    free(encoded_string);
	
    unsigned char hash[32];
    perform_keccak_hash(encoded_hex_array, bytes_amount, hash);

    char hash_string[65] = {0};
    for (int i = 0; i < 32; ++i)
    {
        snprintf(hash_string + i * 2, 3, "%02x", hash[i]);
    }
    printf("Hash string: %s\n", hash_string);

    unsigned char* private_key_hex_array = hex_string_to_hex_array(private_key, strlen(private_key) / 2);
    unsigned char transaction_signature[64];
    uECC_sign((const uint8_t*)private_key_hex_array, (const uint8_t*) hash, 32, transaction_signature, uECC_secp256k1());

    char signature_string[129] = { 0 };
    for (int i = 0; i < 64; ++i)
    {
        snprintf(signature_string + i * 2, 3, "%02x", transaction_signature[i]);
    }
    printf("signature string: %s\n", signature_string);

    char r[65] = { 0 };
    char s[65] = { 0 };
    for (int i = 0; i < 64; i++)
    {
        r[i] = signature_string[i];
        s[i] = signature_string[i + 64];
    }

    //v = (s % 2) + chain_id * 2 + 35
}

unsigned char* hex_string_to_hex_array(const char* hex_string, unsigned int bytes_amount)
{
    unsigned char* encoded_hex_array = malloc(bytes_amount);
    if (encoded_hex_array == NULL)
    {
        fprintf(stderr, "[eth_jrpc_client] Error allocating memory for the request");
        return NULL;
    }

    for (int i = 0; i < bytes_amount * 2; i += 2)
    {
        char encoded_hex_elem[3] = { 0 };
        encoded_hex_elem[0] = hex_string[i];
        encoded_hex_elem[1] = hex_string[i + 1];
        encoded_hex_array[(1 + i) / 2] = (unsigned char)strtol(encoded_hex_elem, NULL, 16);
    }

    return encoded_hex_array;
}