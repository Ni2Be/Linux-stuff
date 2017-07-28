#include "rpc_xdr.h"
#include <rpc/rpc.h>

rpc_response* gen_nums_1_svc(rpc_request *count, struct svc_req *svc)
{
    static rpc_response response;
    for (int i = 0; i < count->count; i++)
    {
        response.result.result_val[i] = i;
    }
    return &response;
}