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
    eth_jrpc_client_init("https://ropsten.infura.io/v3/dc2cd880dea34528a5c8677d77b72e0a");

    const char* call_params[] = { "true", "1000000000", "0xA1720a0E5859f2165fF651744d84B50f03427a09" };
    const char* function_signature = "echoAll(bool,uint256,address)";
    callback_params callback;
    int a = 5000;
    callback.user_callback = testCallback;
    callback.user_callback_data = &a;
    eth_call("0xAdA4d90D22C41eBC084354808E71e91F9b54137D", "0xA1720a0E5859f2165fF651744d84B50f03427a09", function_signature, call_params, callback);
    eth_jrpc_client_cleanup();

    return 0;
}