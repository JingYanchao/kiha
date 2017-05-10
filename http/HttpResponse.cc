//
// Created by jyc on 17-5-10.
//
#include "HttpResponse.h"
#include <muduo/net/Buffer.h>
namespace kiha
{
    void HttpResponse::appendToBuffer(muduo::net::Buffer* output) const
    {
        char buf[32];
        snprintf(buf, sizeof buf, "HTTP/1.1 %d ", statusCode_);
        output->append(buf);
        output->append(statusMessage_);
        output->append("\r\n");

        if (closeConnection_)
        {
            output->append("Connection: close\r\n");
        }
        else
        {
            snprintf(buf, sizeof buf, "Content-Length: %zd\r\n", body_.size());
            output->append(buf);
            output->append("Connection: Keep-Alive\r\n");
            snprintf(buf,sizeof buf,"Server: %s\r\n",Server_.c_str());
            output->append(buf);
        }

        for (std::map<std::string, std::string>::const_iterator it = headers_.begin();
             it != headers_.end(); ++it)
        {
            output->append(it->first);
            output->append(": ");
            output->append(it->second);
            output->append("\r\n");
        }

        output->append("\r\n");
        output->append(body_);
    }
}

