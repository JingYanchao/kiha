//
// Created by jyc on 17-5-4.
//
#include "Http.h"
#include <muduo/base/Logging.h>
namespace kiha
{
    http_parser_settings Http::settings= {
            NULL,               // on_message_begin
            onUrl,              // on_url
            onStatus,           // on_status
            onHeaderField,      // on_header_field
            onHeaderValue,      // on_header_value
            onHeadersComplete,  // on_headers_complete
            NULL,               // on_body
            NULL,               // on_message_complete
            NULL,               // on_chunk_header;
            NULL,               // on_chunk_complete;
            };

    int Http::onHeaderField(HttpParser *parser, const char *data, size_t len)
    {
        Http *http = (Http*)parser->data;
        switch (parser->type)
        {
            case HTTP_REQUEST:
            {
                HttpRequest* request = &http->currentHttprequest;
                request->currentHeaderField.assign(data,len);
                for(char&c:request->currentHeaderField)
                {
                    c = tolower(c);
                }
            }
                break;
            case HTTP_RESPONSE:
            {
                HttpResponse* response = &http->currentHttpresponse;
                response->currentHeaderField.assign(data,len);
                for(char& c: response->currentHeaderField)
                {
                    c = tolower(c);
                }
            }
                break;
            default:
                LOG_FATAL << "HTTP: unknown parser type";
        }
        return 0;
    }

    int Http::onHeaderValue(HttpParser *parser, const char *data, size_t len)
    {
        Http *http = (Http*)parser->data;
        switch (parser->type)
        {

            case HTTP_REQUEST:
            {
                HttpRequest* request = &http->currentHttprequest;
                std::string &headerField = request->currentHeaderField;

                if (headerField.empty())
                {
                    LOG_ERROR << "HTTP: onHeaderValue, no field before";
                    return 1;
                }

                request->headers_[headerField].assign(data, len);
            }
                break;

            case HTTP_RESPONSE:
            {
                HttpResponse* response = &http->currentHttpresponse;
                std::string &headerField = response->currentHeaderField;
                if (headerField.empty())
                {
                    LOG_ERROR << "HTTP: onHeaderValue, no field before";
                    return 1;
                }
                response->headers_[headerField].assign(data, len);
            }
                break;

            default:
                // never reach here
                LOG_FATAL << "HTTP: unknown parser type";
        }
        return 0;
    }

    int Http::onUrl(HttpParser *parser, const char *data, size_t len)
    {
        Http *http = (Http*)parser->data;
        HttpRequest* request = &http->currentHttprequest;
        assert(parser->type == HTTP_REQUEST);
        std::string url(data,len);
        request->setPath(url);
        return 0;
    }

    int Http::onStatus(HttpParser *parser, const char *data, size_t len)
    {
        Http *http = (Http*)parser->data;
        HttpResponse* response = &http->currentHttpresponse;
        assert(parser->type == HTTP_RESPONSE);
        response->setStatusCode(parser->status_code);
        std::string message(data,len);
        response->setStatusMessage(message);
        return 0;
    }

    int Http::onHeadersComplete(HttpParser *parser)
    {
        Http *http = (Http*)parser->data;
        switch (parser->type)
        {
            case HTTP_REQUEST:
            {
                HttpRequest* request = &http->currentHttprequest;
                request->method_ = std::string(http_method_str(
                        (enum http_method) parser->method));

                //this http version is http11
                if(parser->http_major == 1 && parser->http_minor == 1)
                {
                    request->version_ = request->HTTP11;
                }
                //this http version is http10
                else if(parser->http_major == 1 && parser->http_minor == 0)
                {
                    request->version_ = request->HTTP10;
                }
                //this http version is unknown
                else
                {
                    request->version_ = request->UNKNOWN;
                }
                http->state = FINISH;
            }
                break;

            case HTTP_RESPONSE:
            {
                http->state = FINISH;
            }
                break;

            default:
                LOG_FATAL << "HTTP: unknown parser type";
        }
        return 0;
    }

    bool Http::gotAll()
    {
        return state;
    }

    bool Http::parseRequest(muduo::net::Buffer* buf,muduo::Timestamp receiveTime)
    {
        size_t res = http_parser_execute(&context.requestParser, &settings, buf->peek(), buf->readableBytes());
        if(context.requestParser.http_errno!=0)
        {
            LOG_INFO << "HTTP request: "
                      << http_errno_name(HTTP_PARSER_ERRNO(&context.requestParser));
            return false;
        }
        return true;
    }
    void Http::reset()
    {
        http_parser_init(&context.requestParser,HTTP_REQUEST);
        http_parser_init(&context.responseParser,HTTP_RESPONSE);
        context.requestParser.data = this;
        context.responseParser.data = this;
    }
}
