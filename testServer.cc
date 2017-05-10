//
// Created by jyc on 17-5-10.
//
#include "Server.h"
#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
using namespace kiha;
int main()
{
    muduo::Logger::setLogLevel(muduo::Logger::WARN);
    LOG_INFO << "pid = " << getpid();
    muduo::net::EventLoop loop;
    muduo::net::InetAddress listenAddr(2007);
    Server server(&loop, listenAddr);
    server.start();
    loop.loop();
}
