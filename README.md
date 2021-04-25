# ethereum_utils_c

This project is still a work in progress. It consists of a library in C to interact with public nodes in the ethereum blockchain. 
Currently, it provides communication with nodes over the JSON RPC interface, allowing for the encoding and decoding of transactions.
Transaction signing is still in progress.

The main library functions are in the file eth_jrpc_client.c.
It uses libcurl to send and receive the requests from the nodes.
