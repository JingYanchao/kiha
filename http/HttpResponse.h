//
// Created by jyc on 17-5-4.
//

#ifndef HTTPSERVER_HTTPRESPONSE_H
#define HTTPSERVER_HTTPRESPONSE_H

#include "copyable.h"
#include <string>
#include <map>
#include <muduo/net/Buffer.h>
namespace kiha
{
    class HttpResponse:copyable
    {
    public:
        explicit HttpResponse(bool close)
                : statusCode_(200),
                  closeConnection_(close),
                  Server_("kiha")
        {}
        std::string currentHeaderField;

        void setStatusCode(int code)
        { statusCode_ = code; }

        void setStatusMessage(const std::string& message)
        { statusMessage_ = message; }

        void setCloseConnection(bool on)
        { closeConnection_ = on; }

        bool closeConnection() const
        { return closeConnection_; }

        void setContentType(const std::string& contentType)
        { addHeader("Content-Type", contentType); }

        // FIXME: replace string with StringPiece
        void addHeader(const std::string& key, const std::string& value)
        { headers_[key] = value; }

        void  setBody(const std::string& body)
        { body_ = body; }

        void appendToBuffer(muduo::net::Buffer* output) const;
        std::map<std::string, std::string> headers_;

    private:

        int statusCode_;
        // FIXME: add http version
        std::string statusMessage_;
        bool closeConnection_;
        std::string body_;
        const std::string Server_;
    };
}
#endif //HTTPSERVER_HTTPRESPONSE_H
