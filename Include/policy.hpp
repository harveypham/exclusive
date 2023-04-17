#pragma once

template<typename T>
struct nonconst_type
{
    typedef T type;
};

template<typename T>
struct const_type
{
    typedef const T type;
};
