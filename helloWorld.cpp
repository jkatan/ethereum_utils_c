#include <iostream>
#include "eth_jrpc_client.h"
#include "eth_abi_utils.h"

void testCallback(char* response, void* user_data)
{
    std::cout << response << std::endl;
    int* user_int = (int*)user_data;
    std::cout << "User int: " << *user_int;
}

int main(int argc, char* argv[])
{
    eth_jrpc_client_init("https://mainnet.infura.io/v3/dc2cd880dea34528a5c8677d77b72e0a");

    /* callback_params callback;
    int a = 5000;
    callback.user_callback = testCallback;
    callback.user_callback_data = &a;
    eth_getBlockByNumber("latest", 0, callback);

    eth_jrpc_client_cleanup(); */

    const char* call_params[] = {"0xAdA4d90D22C41eBC084354808E71e91F9b54137D"};
    const char* function_signature = "balanceOf(address)";
    encode_eth_call_data(function_signature, call_params);

    return 0;
}