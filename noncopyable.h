//
// Created by jyc on 17-5-4.
//

#ifndef HTTPSERVER_NONCOPYABLE_H
#define HTTPSERVER_NONCOPYABLE_H
namespace kiha
{
    class noncopyable
    {
    protected:
        noncopyable() = default;
        ~noncopyable() = default;

    private:
        noncopyable(const noncopyable&) = delete;
        void operator=(const noncopyable&) = delete;
    };
}
#endif //HTTPSERVER_NONCOPYABLE_H
