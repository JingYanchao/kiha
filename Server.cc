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
            conn->setContext(Http());
            buf->retrieveAll();
        }
    }

    void Server::onRequest(const muduo::net::TcpConnectionPtr & conn, const HttpRequest & request)
    {
        const std::string& connection = request.getHeader("connection");
        bool close = connection == "close" || (request.getVersion() == request.HTTP10&&connection != "Keep-Alive");
        std::string path = "/home/jyc/myproject/kiha/static";
        path+=request.path_;
        HttpResponse response(close);
        response.setStatusCode(200);
        response.setStatusMessage("OK");
        muduo::net::Buffer buff;
        std::string file;
        if(!readFile(path,file))
        {
            response.setStatusCode(404);
            response.setStatusMessage("not Found");
            readFile("/home/jyc/myproject/kiha/static/404page.html",file);
        }
        response.setBody(file);
        response.appendToBuffer(&buff);
//        LOG_INFO<<buff.readableBytes();
        conn->send(&buff);
        if(close)
        {
            conn->shutdown();
        }

    }

    int Server::readFile(const std::string& path, std::string &buf)
    {
        FILE* fp;
        size_t rc;
        char str[65535];
        if((fp = fopen(path.c_str(),"r"))==NULL)
            return 0;
        while((rc = fread(str,sizeof(char),65535,fp))!=0)
        {
            buf.append(str,rc);
        }
        fclose(fp);
        return 1;
    }

}
