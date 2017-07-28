const MAXNUMBERS = 100;

struct rpc_request
{
    int count;
};

struct rpc_response
{
    int result<MAXNUMBERS>;
};

program RPC_TEST
{
    version RPC_VERS
    {
        rpc_response GEN_NUMS (rpc_request) = 1;
    } = 1;
} = 0x2000;