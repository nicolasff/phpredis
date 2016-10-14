#include "php.h"
#include "php_ini.h"

#ifndef REDIS_COMMON_H
#define REDIS_COMMON_H

#if (PHP_MAJOR_VERSION < 7)
#include <ext/standard/php_smart_str.h>
typedef smart_str smart_string;
#define smart_string_0(x) smart_str_0(x)
#define smart_string_appendc(dest, c) smart_str_appendc(dest, c)
#define smart_string_append_long(dest, val) smart_str_append_long(dest, val)
#define smart_string_appendl(dest, src, len) smart_str_appendl(dest, src, len)

typedef struct {
    size_t len;
    char *val;
} zend_string;

#define ZEND_HASH_FOREACH_KEY_VAL(ht, _h, _key, _val) do { \
    HashPosition _hpos; \
    for (zend_hash_internal_pointer_reset_ex(ht, &_hpos); \
         (_val = zend_hash_get_current_data_ex(ht, &_hpos)) != NULL; \
         zend_hash_move_forward_ex(ht, &_hpos) \
    ) { \
        zend_string _zstr = {0}; \
        char *_str_index; uint _str_length; ulong _num_index; \
        switch (zend_hash_get_current_key_ex(ht, &_str_index, &_str_length, &_num_index, 0, &_hpos)) { \
            case HASH_KEY_IS_STRING: \
                _zstr.len = _str_length; \
                _zstr.val = _str_index; \
                _key = &_zstr; \
                break; \
            case HASH_KEY_IS_LONG: \
                _key = NULL; \
                _h = _num_index; \
                break; \
            default: \
                continue; \
        }

#define ZEND_HASH_FOREACH_VAL(ht, _val) do { \
    HashPosition _hpos; \
    for (zend_hash_internal_pointer_reset_ex(ht, &_hpos); \
         (_val = zend_hash_get_current_data_ex(ht, &_hpos)) != NULL; \
         zend_hash_move_forward_ex(ht, &_hpos) \
    ) {

#define ZEND_HASH_FOREACH_END() \
    } \
} while(0)

static zend_always_inline zval *
zend_hash_str_find(const HashTable *ht, const char *key, size_t len)
{
    zval **zv;

    if (zend_hash_find(ht, key, len + 1, (void **)&zv) == SUCCESS) {
        return *zv;
    }
    return NULL;
}

static zend_always_inline void *
zend_hash_str_find_ptr(const HashTable *ht, const char *str, size_t len)
{
    void **ptr;

    if (zend_hash_find(ht, str, len + 1, (void **)&ptr) == SUCCESS) {
        return *ptr;
    }
    return NULL;
}

#undef zend_hash_get_current_data
static zend_always_inline zval *
zend_hash_get_current_data(HashTable *ht)
{
    zval **zv;

    if (zend_hash_get_current_data_ex(ht, (void **)&zv, NULL) == SUCCESS) {
        return *zv;
    }
    return NULL;
}

static zend_always_inline void *
zend_hash_get_current_data_ptr(HashTable *ht)
{
    void **ptr;

    if (zend_hash_get_current_data_ex(ht, (void **)&ptr, NULL) == SUCCESS) {
        return *ptr;
    }
    return NULL;
}

static int (*_zend_hash_index_find)(const HashTable *, ulong, void **) = &zend_hash_index_find;
#define zend_hash_index_find(ht, h) inline_zend_hash_index_find(ht, h)

static zend_always_inline zval *
inline_zend_hash_index_find(const HashTable *ht, zend_ulong h)
{
    zval **zv;
    if (_zend_hash_index_find(ht, h, (void **)&zv) == SUCCESS) {
        return *zv;
    }
    return NULL;
}

static zend_always_inline void *
zend_hash_index_find_ptr(const HashTable *ht, zend_ulong h)
{
    void **ptr;

    if (_zend_hash_index_find(ht, h, (void **)&ptr) == SUCCESS) {
        return *ptr;
    }
    return NULL;
}

static int (*_zend_hash_get_current_data_ex)(HashTable *, void **, HashPosition *) = &zend_hash_get_current_data_ex;
#define zend_hash_get_current_data_ex(ht, pos) inline_zend_hash_get_current_data_ex(ht, pos)
static zend_always_inline zval *
inline_zend_hash_get_current_data_ex(HashTable *ht, HashPosition *pos)
{
    zval **zv;
    if (_zend_hash_get_current_data_ex(ht, (void **)&zv, pos) == SUCCESS) {
        return *zv;
    }
    return NULL;
}

#undef zend_hash_next_index_insert
#define zend_hash_next_index_insert(ht, pData) \
    _zend_hash_next_index_insert(ht, pData ZEND_FILE_LINE_CC)
static zend_always_inline zval *
_zend_hash_next_index_insert(HashTable *ht, zval *pData ZEND_FILE_LINE_DC)
{
    if (_zend_hash_index_update_or_next_insert(ht, 0, &pData, sizeof(pData),
            NULL, HASH_NEXT_INSERT ZEND_FILE_LINE_CC) == SUCCESS
    ) {
        return pData;
    }
    return NULL;
}

#undef zend_get_parameters_array
#define zend_get_parameters_array(ht, param_count, argument_array) \
    inline_zend_get_parameters_array(ht, param_count, argument_array TSRMLS_CC)

static zend_always_inline int
inline_zend_get_parameters_array(int ht, int param_count, zval *argument_array TSRMLS_DC)
{
    int i, ret = FAILURE;
    zval **zv = ecalloc(param_count, sizeof(zval *));

    if (_zend_get_parameters_array(ht, param_count, zv TSRMLS_CC) == SUCCESS) {
        for (i = 0; i < param_count; i++) {
            argument_array[i] = *zv[i];
        }
        ret = SUCCESS;
    }
    efree(zv);
    return ret;
}

typedef zend_rsrc_list_entry zend_resource;

static int (*_add_next_index_stringl)(zval *, const char *, uint, int) = &add_next_index_stringl;
#define add_next_index_stringl(arg, str, length) _add_next_index_stringl(arg, str, length, 1);

#undef RETVAL_STRING
#define RETVAL_STRING(s) ZVAL_STRING(return_value, s, 1)
#undef RETURN_STRING
#define RETURN_STRING(s) { RETVAL_STRING(s); return; }
#undef RETVAL_STRINGL
#define RETVAL_STRINGL(s, l) ZVAL_STRINGL(return_value, s, l, 1)
#undef RETURN_STRINGL
#define RETURN_STRINGL(s, l) { RETVAL_STRINGL(s, l); return; }

#else
#include <ext/standard/php_smart_string.h>
#endif

/* NULL check so Eclipse doesn't go crazy */
#ifndef NULL
#define NULL   ((void *) 0)
#endif

#define redis_sock_name "Redis Socket Buffer"
#define REDIS_SOCK_STATUS_FAILED       0
#define REDIS_SOCK_STATUS_DISCONNECTED 1
#define REDIS_SOCK_STATUS_UNKNOWN      2
#define REDIS_SOCK_STATUS_CONNECTED    3

#define redis_multi_access_type_name "Redis Multi type access"

#define _NL "\r\n"

/* properties */
#define REDIS_NOT_FOUND 0
#define REDIS_STRING    1
#define REDIS_SET       2
#define REDIS_LIST      3
#define REDIS_ZSET      4
#define REDIS_HASH      5

#ifdef PHP_WIN32
#define PHP_REDIS_API __declspec(dllexport)
#else
#define PHP_REDIS_API
#endif

/* reply types */
typedef enum _REDIS_REPLY_TYPE {
    TYPE_EOF       = -1,
    TYPE_LINE      = '+',
    TYPE_INT       = ':',
    TYPE_ERR       = '-',
    TYPE_BULK      = '$',
    TYPE_MULTIBULK = '*'
} REDIS_REPLY_TYPE;

/* SCAN variants */
typedef enum _REDIS_SCAN_TYPE {
    TYPE_SCAN,
    TYPE_SSCAN,
    TYPE_HSCAN,
    TYPE_ZSCAN
} REDIS_SCAN_TYPE;

/* PUBSUB subcommands */
typedef enum _PUBSUB_TYPE {
    PUBSUB_CHANNELS,
    PUBSUB_NUMSUB,
    PUBSUB_NUMPAT
} PUBSUB_TYPE;

/* options */
#define REDIS_OPT_SERIALIZER         1
#define REDIS_OPT_PREFIX             2
#define REDIS_OPT_READ_TIMEOUT       3
#define REDIS_OPT_SCAN               4

/* cluster options */
#define REDIS_OPT_FAILOVER               5
#define REDIS_FAILOVER_NONE              0
#define REDIS_FAILOVER_ERROR             1
#define REDIS_FAILOVER_DISTRIBUTE        2
#define REDIS_FAILOVER_DISTRIBUTE_SLAVES 3
/* serializers */
#define REDIS_SERIALIZER_NONE        0
#define REDIS_SERIALIZER_PHP         1
#define REDIS_SERIALIZER_IGBINARY    2

/* SCAN options */
#define REDIS_SCAN_NORETRY 0
#define REDIS_SCAN_RETRY 1

/* GETBIT/SETBIT offset range limits */
#define BITOP_MIN_OFFSET 0
#define BITOP_MAX_OFFSET 4294967295

/* Specific error messages we want to throw against */
#define REDIS_ERR_LOADING_MSG "LOADING Redis is loading the dataset in memory"
#define REDIS_ERR_LOADING_KW  "LOADING"
#define REDIS_ERR_AUTH_MSG    "NOAUTH Authentication required."
#define REDIS_ERR_AUTH_KW     "NOAUTH"
#define REDIS_ERR_SYNC_MSG    "MASTERDOWN Link with MASTER is down and slave-serve-stale-data is set to 'no'"
#define REDIS_ERR_SYNC_KW     "MASTERDOWN"

#define IF_MULTI() if(redis_sock->mode == MULTI)
#define IF_MULTI_OR_ATOMIC() if(redis_sock->mode == MULTI || redis_sock->mode == ATOMIC)\

#define IF_MULTI_OR_PIPELINE() if(redis_sock->mode == MULTI || redis_sock->mode == PIPELINE)
#define IF_PIPELINE() if(redis_sock->mode == PIPELINE)
#define IF_NOT_PIPELINE() if(redis_sock->mode != PIPELINE)
#define IF_NOT_MULTI() if(redis_sock->mode != MULTI)
#define IF_NOT_ATOMIC() if(redis_sock->mode != ATOMIC)
#define IF_ATOMIC() if(redis_sock->mode == ATOMIC)
#define ELSE_IF_MULTI() else IF_MULTI() { \
    if(redis_response_enqueued(redis_sock TSRMLS_CC) == 1) { \
        RETURN_ZVAL(getThis(), 1, 0);\
    } else { \
        RETURN_FALSE; \
    } \
}

#define ELSE_IF_PIPELINE() else IF_PIPELINE() { \
    RETURN_ZVAL(getThis(), 1, 0);\
}

#define MULTI_RESPONSE(callback) IF_MULTI_OR_PIPELINE() { \
    fold_item *f1, *current; \
    f1 = malloc(sizeof(fold_item)); \
    f1->fun = (void *)callback; \
    f1->next = NULL; \
    current = redis_sock->current;\
    if(current) current->next = f1; \
    redis_sock->current = f1; \
  }

#define PIPELINE_ENQUEUE_COMMAND(cmd, cmd_len) request_item *tmp; \
    struct request_item *current_request;\
    tmp = malloc(sizeof(request_item));\
    tmp->request_str = calloc(cmd_len, 1);\
    memcpy(tmp->request_str, cmd, cmd_len);\
    tmp->request_size = cmd_len;\
    tmp->next = NULL;\
    current_request = redis_sock->pipeline_current; \
    if(current_request) {\
        current_request->next = tmp;\
    } \
    redis_sock->pipeline_current = tmp; \
    if(NULL == redis_sock->pipeline_head) { \
        redis_sock->pipeline_head = redis_sock->pipeline_current;\
    }

#define SOCKET_WRITE_COMMAND(redis_sock, cmd, cmd_len) \
    if(redis_sock_write(redis_sock, cmd, cmd_len TSRMLS_CC) < 0) { \
    efree(cmd); \
    RETURN_FALSE; \
}

#define REDIS_SAVE_CALLBACK(callback, closure_context) \
    IF_MULTI_OR_PIPELINE() { \
        fold_item *f1, *current; \
        f1 = malloc(sizeof(fold_item)); \
        f1->fun = (void *)callback; \
        f1->ctx = closure_context; \
        f1->next = NULL; \
        current = redis_sock->current;\
        if(current) current->next = f1; \
        redis_sock->current = f1; \
        if(NULL == redis_sock->head) { \
            redis_sock->head = redis_sock->current;\
        }\
}

#define REDIS_ELSE_IF_MULTI(function, closure_context) \
    else IF_MULTI() { \
        if(redis_response_enqueued(redis_sock TSRMLS_CC) == 1) {\
            REDIS_SAVE_CALLBACK(function, closure_context); \
            RETURN_ZVAL(getThis(), 1, 0);\
        } else {\
            RETURN_FALSE;\
        }\
}

#define REDIS_ELSE_IF_PIPELINE(function, closure_context) \
    else IF_PIPELINE() { \
        REDIS_SAVE_CALLBACK(function, closure_context); \
        RETURN_ZVAL(getThis(), 1, 0); \
}

#define REDIS_PROCESS_REQUEST(redis_sock, cmd, cmd_len) \
    IF_PIPELINE() { \
        PIPELINE_ENQUEUE_COMMAND(cmd, cmd_len); \
    } else { \
        SOCKET_WRITE_COMMAND(redis_sock, cmd, cmd_len); \
    } \
    efree(cmd);

#define REDIS_PROCESS_RESPONSE_CLOSURE(function, closure_context) \
    REDIS_ELSE_IF_MULTI(function, closure_context) \
    REDIS_ELSE_IF_PIPELINE(function, closure_context);

#define REDIS_PROCESS_RESPONSE(function) \
    REDIS_PROCESS_RESPONSE_CLOSURE(function, NULL)

/* Clear redirection info */
#define REDIS_MOVED_CLEAR(redis_sock) \
    redis_sock->redir_slot = 0; \
    redis_sock->redir_port = 0; \
    redis_sock->redir_type = MOVED_NONE; \

/* Process a command assuming our command where our command building
 * function is redis_<cmdname>_cmd */
#define REDIS_PROCESS_CMD(cmdname, resp_func) \
    RedisSock *redis_sock; char *cmd; int cmd_len; void *ctx=NULL; \
    if(redis_sock_get(getThis(), &redis_sock TSRMLS_CC, 0)<0 || \
       redis_##cmdname##_cmd(INTERNAL_FUNCTION_PARAM_PASSTHRU,redis_sock, \
                             &cmd, &cmd_len, NULL, &ctx)==FAILURE) { \
            RETURN_FALSE; \
    } \
    REDIS_PROCESS_REQUEST(redis_sock, cmd, cmd_len); \
    IF_ATOMIC() { \
        resp_func(INTERNAL_FUNCTION_PARAM_PASSTHRU, redis_sock, NULL, ctx); \
    } \
    REDIS_PROCESS_RESPONSE_CLOSURE(resp_func,ctx);

/* Process a command but with a specific command building function 
 * and keyword which is passed to us*/
#define REDIS_PROCESS_KW_CMD(kw, cmdfunc, resp_func) \
    RedisSock *redis_sock; char *cmd; int cmd_len; void *ctx=NULL; \
    if(redis_sock_get(getThis(), &redis_sock TSRMLS_CC, 0)<0 || \
       cmdfunc(INTERNAL_FUNCTION_PARAM_PASSTHRU, redis_sock, kw, &cmd, \
               &cmd_len, NULL, &ctx)==FAILURE) { \
            RETURN_FALSE; \
    } \
    REDIS_PROCESS_REQUEST(redis_sock, cmd, cmd_len); \
    IF_ATOMIC() { \
        resp_func(INTERNAL_FUNCTION_PARAM_PASSTHRU, redis_sock, NULL, ctx); \
    } \
    REDIS_PROCESS_RESPONSE_CLOSURE(resp_func,ctx);

#define REDIS_STREAM_CLOSE_MARK_FAILED(redis_sock) \
    redis_stream_close(redis_sock TSRMLS_CC); \
    redis_sock->stream = NULL; \
    redis_sock->mode = ATOMIC; \
    redis_sock->status = REDIS_SOCK_STATUS_FAILED; \
    redis_sock->watching = 0

/* Extended SET argument detection */
#define IS_EX_ARG(a) \
    ((a[0]=='e' || a[0]=='E') && (a[1]=='x' || a[1]=='X') && a[2]=='\0')
#define IS_PX_ARG(a) \
    ((a[0]=='p' || a[0]=='P') && (a[1]=='x' || a[1]=='X') && a[2]=='\0')
#define IS_NX_ARG(a) \
    ((a[0]=='n' || a[0]=='N') && (a[1]=='x' || a[1]=='X') && a[2]=='\0')
#define IS_XX_ARG(a) \
    ((a[0]=='x' || a[0]=='X') && (a[1]=='x' || a[1]=='X') && a[2]=='\0')

#define IS_EX_PX_ARG(a) (IS_EX_ARG(a) || IS_PX_ARG(a))
#define IS_NX_XX_ARG(a) (IS_NX_ARG(a) || IS_XX_ARG(a))

/* Given a string and length, validate a zRangeByLex argument.  The semantics
 * here are that the argument must start with '(' or '[' or be just the char
 * '+' or '-' */
#define IS_LEX_ARG(s,l) \
    (l>0 && (*s=='(' || *s=='[' || (l==1 && (*s=='+' || *s=='-'))))

typedef enum {ATOMIC, MULTI, PIPELINE} redis_mode;

typedef struct fold_item {
    zval * (*fun)(INTERNAL_FUNCTION_PARAMETERS, void *, ...);
    void *ctx;
    struct fold_item *next;
} fold_item;

typedef struct request_item {
    char *request_str; 
    int request_size; /* size_t */
    struct request_item *next;
} request_item;

/* {{{ struct RedisSock */
typedef struct {
    php_stream     *stream;
    char           *host;
    short          port;
    char           *auth;
    double         timeout;
    double         read_timeout;
    long           retry_interval;
    int            failed;
    int            status;
    int            persistent;
    int            watching;
    char           *persistent_id;

    int            serializer;
    long           dbNumber;

    char           *prefix;
    int            prefix_len;

    redis_mode     mode;
    fold_item      *head;
    fold_item      *current;

    request_item   *pipeline_head;
    request_item   *pipeline_current;

    char           *err;
    int            err_len;
    zend_bool      lazy_connect;

    int            scan;

    int            readonly;
} RedisSock;
/* }}} */

void
free_reply_callbacks(zval *z_this, RedisSock *redis_sock);

#endif
