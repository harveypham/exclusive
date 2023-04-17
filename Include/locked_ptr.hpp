#pragma once

#ifdef _DEBUG
#include <iostream>
#endif

#include <mutex>

#include "policy.hpp"

namespace locked_ptr_impl
{
    using namespace std;

    template<typename _Resource, typename _Mutex, typename _Guard, template<class> class const_policy>
    class locked_ptr_base
    {
        typedef typename const_policy<_Resource>::type _ResourceType;

    public:
        locked_ptr_base(_ResourceType* resource, _Mutex& mutex)
            : resource_(resource), guard_(mutex)
        {
#ifdef _DEBUG
            cout << "Acquired resource" << endl;
#endif
        }

#ifdef _DEBUG
        locked_ptr_base(locked_ptr_base<_Resource, _Mutex, _Guard, const_policy>&& ptr)
            : resource_(move(ptr.resource_)), guard_(move(ptr.guard_))
        {
            cout << "Moved accquired resource" << endl;
        }
#else
        locked_ptr_base(locked_ptr_base<_Resource, _Mutex, _Guard, const_policy>&& ptr) = default;
#endif

        locked_ptr_base() = delete;
        locked_ptr_base(const locked_ptr_base<_Resource, _Mutex, _Guard, const_policy>&) = delete;
        locked_ptr_base<_Resource, _Mutex, _Guard, const_policy>& operator=(const locked_ptr_base<_Resource, _Mutex, _Guard, const_policy>&) = delete;

        ~locked_ptr_base()
        {
#ifdef _DEBUG
            cout << "Released resource" << endl;
#endif
        }

        // Accessing locked resource
        _ResourceType* operator->()
        {
            return resource_;
        }

    private:

        _ResourceType* resource_;
        _Guard guard_;
    };
}

template<typename _Resource, typename _Mutex = std::mutex, typename _Guard = std::lock_guard<_Mutex>>
using locked_ptr = typename locked_ptr_impl::locked_ptr_base < _Resource, _Mutex, _Guard, nonconst_type >;

template<typename _Resource, typename _Mutex = std::mutex, typename _Guard = std::lock_guard<_Mutex>>
using const_locked_ptr = typename locked_ptr_impl::locked_ptr_base < _Resource, _Mutex, _Guard, const_type >;
