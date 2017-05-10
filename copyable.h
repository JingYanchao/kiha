//
// Created by jyc on 17-5-4.
//

#ifndef HTTPSERVER_COPYABLE_H
#define HTTPSERVER_COPYABLE_H
namespace kiha
{

/// A tag class emphasises the objects are copyable.
/// The empty base class optimization applies.
/// Any derived class of copyable should be a value type.
    class copyable
    {
    protected:
        copyable() = default;
        ~copyable() = default;
    };

};
#endif //HTTPSERVER_COPYABLE_H
