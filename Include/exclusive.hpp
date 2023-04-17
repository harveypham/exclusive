#pragma once

#include <memory>

#include "locked_ptr.hpp"

namespace exclusive_impl {

    using namespace std;

    template<typename _Resource, typename _Mutex, typename _Guard = lock_guard<_Mutex>>
    class exclusive_obj
    {
    public:
        template<typename T, typename ... Args>
        exclusive_obj(T arg1, Args ... args) : resource_(arg1, args...)
        {}

        // Constructing exclusive resource by moving a non-protected resource to this
        exclusive_obj(_Resource&& resource)
            : resource_(resource) {}

        exclusive_obj() = delete;
        exclusive_obj(const exclusive_obj<_Resource, _Mutex, _Guard>&) = delete;
        exclusive_obj<_Resource, _Mutex, _Guard>& operator=(const exclusive_obj<_Resource, _Mutex, _Guard>&) = delete;

        // Acquire mechanism
        ::locked_ptr<_Resource, _Mutex, _Guard> acquire()
        {
            return ::locked_ptr<_Resource, _Mutex, _Guard>(&resource_, mutex_);
        }

        const_locked_ptr<_Resource, _Mutex, _Guard> acquire() const
        {
            return const_locked_ptr<_Resource, _Mutex, _Guard>(&resource_, mutex_);
        }

        ::locked_ptr<_Resource, _Mutex, _Guard> operator->()
        {
            return acquire();
        }

        const_locked_ptr<_Resource, _Mutex, _Guard> operator->() const
        {
            return acquire();
        }

    private:

        _Resource resource_;
        mutable _Mutex mutex_;
    };

    template<typename _Resource, typename _Mutex, typename _Guard = lock_guard<_Mutex>>
    class exclusive_ptr
    {
    public:
        template<typename T, typename ... Args>
        exclusive_ptr(T arg1, Args ... args) : resource_(make_unique<_Resource>(arg1, args...))
        {}

        // Contructing exclusive resource from unsafe pointer
        exclusive_ptr(_Resource* resource)
            : resource_(resource) {}

        exclusive_ptr() = delete;
        exclusive_ptr(const exclusive_ptr<_Resource, _Mutex, _Guard>&) = delete;
        exclusive_ptr<_Resource, _Mutex, _Guard>& operator=(const exclusive_ptr<_Resource, _Mutex, _Guard>&) = delete;

        // Checkout mechanism
        ::locked_ptr<_Resource, _Mutex, _Guard> acquire()
        {
            return ::locked_ptr<_Resource, _Mutex, _Guard>(resource_.get(), mutex_);
        }

        ::const_locked_ptr<_Resource, _Mutex, _Guard> acquire() const
        {
            return ::const_locked_ptr<_Resource, _Mutex, _Guard>(resource_.get(), mutex_);
        }

        ::locked_ptr< _Resource, _Mutex, _Guard > operator->()
        {
            return acquire();
        }

        ::const_locked_ptr < _Resource, _Mutex, _Guard > operator->() const
        {
            return acquire();
        }

    private:

        unique_ptr<_Resource> resource_;
        mutable _Mutex mutex_;
    };

}

template<typename _Resource, typename _Mutex = std::mutex, typename _Guard = std::lock_guard<_Mutex>>
using exclusive_obj = exclusive_impl::exclusive_obj < _Resource, _Mutex, _Guard >;

template<typename _Resource, typename _Mutex = std::mutex, typename _Guard = std::lock_guard<_Mutex>>
using exclusive_ptr = exclusive_impl::exclusive_ptr < _Resource, _Mutex, _Guard >;
