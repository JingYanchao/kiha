//
// Created by jyc on 17-5-3.
//
#include "Server.h"
#include <muduo/base/Logging.h>
#include <string>
using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;
namespace kiha
{
    Server::Server(muduo::net::EventLoop* loop,
                    const muduo::net::InetAddress& listenAddr):
                server_(loop,listenAddr,"HttpServer",muduo::net::TcpServer::kReusePort)
    {
        server_.setConnectionCallback(
                std::bind(&Server::onConnection,this,_1));
        server_.setMessageCallback(
                std::bind(&Server::onMessage,this,_1,_2,_3));
    }
    void Server::start()
    {
        server_.start();
    }

    void Server::onConnection(const muduo::net::TcpConnectionPtr &conn)
    {
        if (conn->connected())
        {
            conn->setContext(Http());
        }
    }

    void Server::onMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buf, muduo::Timestamp time)
    {
        Http* http = boost::any_cast<Http>(conn->getMutableContext());
        LOG_INFO<<http->currentHttprequest.path_.size();
        http->context.requestParser.data = http;
        http->context.responseParser.data = http;

        if(!http->parseRequest(buf,time))
        {
            //encounter error when parser the http request
            //TODO add the error page
            conn->send("HTTP/1.1 400 Bad Request\r\n\r\n");
            conn->shutdown();
        }
        if(http->gotAll())
        {
            //got the complete infomation
            onRequest(conn,http->currentHttprequest);
            //TODO add the reset
            http->reset();
        }
    }
    void Server::onRequest(const muduo::net::TcpConnectionPtr & conn, const HttpRequest & request)
    {
        LOG_INFO<<request.method();
    }

}
