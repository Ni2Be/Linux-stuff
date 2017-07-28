#include "rpc_xdr.h"
#include <rpc/rpc.h>

int main(int agrc, char** argv)
{
    int i; 
    CLIENT *client;
    rpc_request request;
    rpc_response *response;
    client = cln_create(argv[1], RPC_TEST, 1, "tcp");
    if (client == NULL) 
    {
        printf("shit\n");
        exit(-1);
    }

    int number_count = 10;
    request.count = number_count;

    response = gen_nums_1(&request, client);

    for (int i = 0; i < number_count; i++)
    {
        printf("num: %d\n", response->result.result_val[i]);
    }

    xdr_free(xdr_rpc_response, response);
}