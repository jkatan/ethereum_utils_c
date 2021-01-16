#include <iostream>
#include "eth_jrpc_client.h"
#include "eth_rlp_utils.h"

void testCallback(char* response, void* user_data)
{
    std::cout << response << std::endl;
}

int main(int argc, char* argv[])
{
    /*eth_jrpc_client_init("https://ropsten.infura.io/v3/dc2cd880dea34528a5c8677d77b72e0a");

    const char* call_params[] = { "true", "7", "0xA1720a0E5859f2165fF651744d84B50f03427a09" };
    const char* function_signature = "echoAll(bool,uint256,address)";
    callback_params callback;
    callback.user_callback = testCallback;
    eth_call("0xAdA4d90D22C41eBC084354808E71e91F9b54137D", "0xA1720a0E5859f2165fF651744d84B50f03427a09", function_signature, call_params, callback);
    eth_jrpc_client_cleanup();*/

    char* encoding_test = rlp_encode_hex_string("7917bc33eea648809c285607579c9919fb864f8f");
    printf("value: %s", encoding_test);
    return 0;
}