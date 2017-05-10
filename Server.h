//
// Created by jyc on 17-5-3.
//

#ifndef HTTPSERVER_SERVER_H
#define HTTPSERVER_SERVER_H

#include "http/Http.h"
#include <muduo/net/TcpServer.h>

namespace kiha
{
    class Server
    {
    public:
        Server(muduo::net::EventLoop* loop,
               const muduo::net::InetAddress& listenAddr);

        void start();  // calls server_.start();
    private:
        void onConnection(const muduo::net::TcpConnectionPtr& conn);

        void onMessage(const muduo::net::TcpConnectionPtr& conn,
                       muduo::net::Buffer* buf,
                       muduo::Timestamp time);
        void onRequest(const muduo::net::TcpConnectionPtr&, const HttpRequest&);

        muduo::net::TcpServer server_;
    };
}




#endif //HTTPSERVER_SERVER_H
