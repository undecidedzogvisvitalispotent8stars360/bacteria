#include "json_rpc.h"
/*
addmultisigaddress <nrequired> <'["key","key"]'> [account]
addnode <node> <add|remove|onetry>
backupwallet <destination>
createmultisig <nrequired> <'["key","key"]'>
createrawtransaction [{"txid":txid,"vout":n},...] {address:amount,...}
decoderawtransaction <hex string>
dumpprivkey <gostcoinaddress>
encryptwallet <passphrase>
getaccount <gostcoinaddress>
getaccountaddress <account>
getaddednodeinfo <dns> [node]
getaddressesbyaccount <account>
getbalance [account] [minconf=1]
getbestblockhash
getblock <hash> [verbose=true]
getblockcount
getblockhash <index>
getblocktemplate [params]
getconnectioncount
getdifficulty
getgenerate
gethashespersec
getinfo
getmininginfo
getnetworkhashps [blocks] [height]
getnewaddress [account]
getpeerinfo
getrawmempool
getrawtransaction <txid> [verbose=0]
getreceivedbyaccount <account> [minconf=1]
getreceivedbyaddress <gostcoinaddress> [minconf=1]
gettransaction <txid>
gettxout <txid> <n> [includemempool=true]
gettxoutsetinfo
getwork [data]
getworkex [data, coinbase]
help [command]
importprivkey <gostcoinprivkey> [label] [rescan=true]
keypoolrefill
listaccounts [minconf=1]
listaddressgroupings
listlockunspent
listreceivedbyaccount [minconf=1] [includeempty=false]
listreceivedbyaddress [minconf=1] [includeempty=false]
listsinceblock [blockhash] [target-confirmations]
listtransactions [account] [count=10] [from=0]
listunspent [minconf=1] [maxconf=9999999] ["address",...]
lockunspent unlock? [array-of-Objects]
move <fromaccount> <toaccount> <amount> [minconf=1] [comment]
sendfrom <fromaccount> <togostcoinaddress> <amount> [minconf=1] [comment]
[comment-to] sendmany <fromaccount> {address:amount,...} [minconf=1] [comment]
sendrawtransaction <hex string>
sendtoaddress <gostcoinaddress> <amount> [comment] [comment-to]
setaccount <gostcoinaddress> <account>
setgenerate <generate> [genproclimit]
setmininput <amount>
settxfee <amount>
signmessage <gostcoinaddress> <message>
signrawtransaction <hex string>
[{"txid":txid,"vout":n,"scriptPubKey":hex,"redeemScript":hex},...]
[<privatekey1>,...] [sighashtype="ALL"] stop submitblock <hex data>
[optional-params-obj] validateaddress <gostcoinaddress> verifychain [check
level] [num blocks] verifymessage <gostcoinaddress> <signature> <message>

*/
//#define RPCBUFMAX 2056

json_t *brpc_prepare_params(char *data, ...) {
  json_t *ret = json_array();
  va_list ap;
  va_start(ap, data);
  while (data != NULL) {
    json_array_append(ret, json_string(data));
    data = va_arg(ap, char *);
  }
  va_end(ap);
  return ret;
}

void brpc_get_json(struct bitcoin_rpc_data *data) {
  char *s = NULL;

  json_t *root = json_object();
  json_t *params = data->params;

  json_object_set_new(root, "jsonrpc", json_integer(1));
  json_object_set_new(root, "id", json_string("bacteria"));
  json_object_set_new(root, "method", json_string(data->method));
  json_object_set_new(root, "params", params);

  s = json_dumps(root, 0);
  // puts(s);
  data->json_ret = calloc(sizeof(char), strlen(s) + 1);
  strcpy(data->json_ret, s);

  json_decref(root);
}
void brpc_clear_bdata(struct bitcoin_rpc_data *data) { free(data->json_ret); }

extern void init_string(struct string *s);

extern size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s);
json_t *brpc_json_request(struct cryptocoin *c,
                          struct bitcoin_rpc_data *bdata) {
  CURL *curl = curl_easy_init();
  struct curl_slist *headers = NULL;
  json_t *ret = NULL;
  json_error_t error;
  if (curl) {
    char userpwd[strlen(c->rpcuser) + strlen(c->rpcpassword) + 2];
    char url[strlen(c->rpchost) + sizeof("http://") + sizeof("65535") + 2];
    sprintf(userpwd, "%s:%s%c", c->rpcuser, c->rpcpassword, 0);
    sprintf(url, "http://%s:%d%c", c->rpchost, c->rpcport, 0);

    struct string s;
    init_string(&s);

    const char *data = bdata->json_ret;
    headers = curl_slist_append(headers, "content-type: text/plain;");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_URL, url);

    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(data));
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

    curl_easy_setopt(curl, CURLOPT_USERPWD, userpwd);

    curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_TRY);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);

    curl_easy_perform(curl);
    brpc_clear_bdata(bdata);
    // printf("%s\n", s.ptr);
    ret = json_loads(s.ptr, 0, &error);
    free(s.ptr);
    s.len = 0;
    if (!ret) {
      fprintf(stderr, "JSON-RPC; Error decode; Maybe unathorized %d -> %s\n",
              error.line, error.text);
    }
  }
  curl_slist_free_all(headers);
  curl_easy_cleanup(curl);

  return ret;
}
