//
// Created by jyc on 17-5-4.
//

#ifndef HTTPSERVER_HTTPREQUEST_H
#define HTTPSERVER_HTTPREQUEST_H

#include "../copyable.h"
#include <string>
#include <map>
#include <muduo/base/Timestamp.h>
#include <muduo/base/Logging.h>
namespace kiha
{
    class HttpRequest:copyable
    {
    public:
        std::string currentHeaderField;
        enum Version
        {
            UNKNOWN,HTTP10,HTTP11
        };
        enum HeaderState
        {
            BEGIN,FIELD,VALUE
        };

        HttpRequest():
                method_("INVALID"),version_(UNKNOWN),state_(BEGIN){}

        void setVersion(Version v)
        {
            version_ = v;
        }
        Version getVersion()const
        {
            return version_;
        }

        bool setMethod(std::string& mtd)
        {
            assert(method_=="INVALID");
            if(mtd == "GET")
            {
                method_ = "GET";
            }
            else if(mtd == "POST")
            {
                method_ = "POST";
            }
            else if(mtd == "HEAD")
            {
                method_ = "HEAD";
            }
            else if(mtd == "PUT")
            {
                method_ = "PUT";
            }
            else if(mtd == "DELETE")
            {
                method_ = "DELETE";
            }
            else
            {
                method_ = "INVALID";
            }
            return method_ !="INVALID";
        }

        std::string method() const
        {
            return method_;
        }

        void setPath(std::string& path)
        {
            path_.assign(path);
        }

        const std::string& path() const
        {
            return path_;
        }

        void setQuery(std::string& query)
        {
            query_.assign(query);
        }
        const std::string& query() const
        {
            return query_;
        }
        void setReceiveTime(muduo::Timestamp t)
        {
            receiveTime_ = t;
        }

        muduo::Timestamp receiveTime() const
        {
            return receiveTime_;
        }

        void addHeader(std::string& field,std::string& value)
        {
            headers_[field] = value;
        }

        std::string getHeader(const std::string& field) const
        {
            std::string result;
            auto It = headers_.find(field);
            if(It!=headers_.end())
            {
                result = It->second;
            }
            return result;
        }

        const std::map<std::string,std::string>& headers() const
        {
            return headers_;
        };
        std::string method_;
        Version version_;
        HeaderState state_;
        std::string path_;
        std::string query_;
        muduo::Timestamp receiveTime_;
        std::map<std::string, std::string> headers_;

    };
}



#endif //HTTPSERVER_HTTPREQUEST_H
