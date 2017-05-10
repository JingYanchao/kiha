//
// Created by jyc on 17-5-4.
//

#ifndef HTTPSERVER_HTTP_H
#define HTTPSERVER_HTTP_H

#include "../noncopyable.h"
#include "HttpParser.h"
#include "HttpResponse.h"
#include "HttpRequest.h"
#include <muduo/net/Buffer.h>
#include <muduo/base/Timestamp.h>
namespace kiha
{
    struct HttpContext
    {
        HttpContext()
        {
            http_parser_init(&requestParser, HTTP_REQUEST);
            http_parser_init(&responseParser, HTTP_RESPONSE);
        }
        HttpParser requestParser;
        HttpParser responseParser;
    };

    class Http
    {
    public:
        enum State
        {
            START = 0,
            FINISH = 1
        };
        Http():currentHttpresponse(0)
        {
            context.responseParser.data = this;
            context.requestParser.data = this;
        }
        static int onUrl(HttpParser* parser,const char* data,size_t len);
        static int onStatus(HttpParser *parser,const char* data,size_t len);
        static int onHeaderField(HttpParser *parser, const char *data, size_t len);
        static int onHeaderValue(HttpParser *parser, const char *data, size_t len);
        static int onHeadersComplete(HttpParser *parser);
        static http_parser_settings settings;
        void reset();
        bool gotAll();
        bool parseRequest(muduo::net::Buffer* buf,muduo::Timestamp receiveTime);
        State state;
        HttpRequest currentHttprequest;
        HttpResponse currentHttpresponse;
        HttpContext context;
    private:

    };


}
#endif //HTTPSERVER_HTTP_H
