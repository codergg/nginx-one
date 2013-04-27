//通过vim ngx_http_hello_module.c 创建文件并录入下面的内容

    //这3个文件必须导入,很多有用的方法都定义在里面.
    #include <ngx_config.h>
    #include <ngx_core.h>
    #include <ngx_http.h>

    //声明配置方法
    static char *ngx_diskone(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

    //声明这个模块能够识别的配置信息
    static ngx_command_t  ngx_diskone_commands[] = {
    { ngx_string("hello"),
      NGX_HTTP_LOC_CONF|NGX_CONF_NOARGS, //仅位于Location里,没有额外参数
      ngx_diskone,                                       //指向配置方法
      NGX_HTTP_LOC_CONF_OFFSET,
      0,
      NULL }
    };

    //定义模块模块上下文,即在nginx解释配置信息时的行为,这里全部为NULL
    static ngx_http_module_t  ngx_diskone_module_ctx = {
        NULL,                               /* preconfiguration */
        NULL,                               /* postconfiguration */

        NULL,                               /* create main configuration */
        NULL,                               /* init main configuration */

        NULL,                               /* create server configuration */
        NULL,                               /* merge server configuration */

        NULL,                   /* create location configuration */
        NULL                                /* merge location configuration */
    };

    //定义本模块
    ngx_module_t  ngx_diskone_module = {
        NGX_MODULE_V1,
        &ngx_diskone_module_ctx,      /* module context 模块上下文*/
        ngx_diskone_commands,         /* module directives  这个模块能够识别的配置信息*/
        NGX_HTTP_MODULE,               /* module type 一般都是HTTP模块的啦*/ 
        NULL,                          /* init master */
        NULL,                          /* init module */
        NULL,                          /* init process */
        NULL,                          /* init thread */
        NULL,                          /* exit thread */
        NULL,                          /* exit process */
        NULL,                          /* exit master */
        NGX_MODULE_V1_PADDING
    };

    //请求处理器,由ngx_http_hello方法向nginx登记
    static ngx_int_t
    ngx_diskone_handler(ngx_http_request_t *r){
        //ngx_int_t                  rc;
        size_t             size;
        ngx_chain_t        out;
        ngx_buf_t         *b;

        ngx_str_set(&r->headers_out.content_type, "text/plain");//设置响应的类型为纯文本
        size = sizeof("It is OK");
        b = ngx_create_temp_buf(r->pool, size); //获取内存空间
        if (b == NULL) {
            return NGX_HTTP_INTERNAL_SERVER_ERROR;
        }
        b->last = ngx_sprintf(b->last, "It is OK!"); //写入内容到缓存
        out.buf = b;                                        //将响应体的内容指向b缓存
        out.next = NULL;

        r->headers_out.status = NGX_HTTP_OK; //响应码,200
        r->headers_out.content_length_n = b->last - b->pos;//设置响应体的长度

        b->last_buf = 1;
 
        ngx_http_send_header(r);           //发送响应头,如Content-Type    text/plain

        return ngx_http_output_filter(r, &out);//继续其他过滤器
    };

    //将本模块的处理方法ngx_http_hello_handler登记到nginx配置上下文.
    static char *ngx_diskone(ngx_conf_t *cf, ngx_command_t *cmd, void *conf){
        ngx_http_core_loc_conf_t  *clcf;

        clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
        clcf->handler = ngx_diskone_handler;

        return NGX_CONF_OK;
    };
