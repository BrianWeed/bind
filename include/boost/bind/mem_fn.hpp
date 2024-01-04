#ifndef BOOST_BIND_MEM_FN_HPP_INCLUDED
#define BOOST_BIND_MEM_FN_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

//
//  mem_fn.hpp - a generalization of std::mem_fun[_ref]
//
//  Copyright (c) 2001, 2002 Peter Dimov and Multi Media Ltd.
//  Copyright (c) 2001 David Abrahams
//  Copyright (c) 2003-2005 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/bind/mem_fn.html for documentation.
//

#include <boost/get_pointer.hpp>
#include <boost/config.hpp>
#include <boost/config/workaround.hpp>
#include <type_traits>

namespace boost
{

namespace _mfi
{

template<class T> struct remove_cvref: std::remove_cv< typename std::remove_reference<T>::type >
{
};

} // namespace _mfi

namespace _mfi
{

#define BOOST_MEM_FN_NAME(X) X
#define BOOST_MEM_FN_CC

#include <boost/bind/mem_fn_template.hpp>

#undef BOOST_MEM_FN_CC
#undef BOOST_MEM_FN_NAME

#if defined(BOOST_MEM_FN_ENABLE_CDECL) && !defined(_M_X64)

#define BOOST_MEM_FN_NAME(X) X##_cdecl
#define BOOST_MEM_FN_CC __cdecl

#include <boost/bind/mem_fn_template.hpp>

#undef BOOST_MEM_FN_CC
#undef BOOST_MEM_FN_NAME

#endif

#if defined(BOOST_MEM_FN_ENABLE_STDCALL) && !defined(_M_X64)

#define BOOST_MEM_FN_NAME(X) X##_stdcall
#define BOOST_MEM_FN_CC __stdcall

#include <boost/bind/mem_fn_template.hpp>

#undef BOOST_MEM_FN_CC
#undef BOOST_MEM_FN_NAME

#endif

#if defined(BOOST_MEM_FN_ENABLE_FASTCALL) && !defined(_M_X64)

#define BOOST_MEM_FN_NAME(X) X##_fastcall
#define BOOST_MEM_FN_CC __fastcall

#include <boost/bind/mem_fn_template.hpp>

#undef BOOST_MEM_FN_CC
#undef BOOST_MEM_FN_NAME

#endif

} // namespace _mfi

//

#define BOOST_MEM_FN_NAME(X) X
#define BOOST_MEM_FN_CC
#define BOOST_MEM_FN_NOEXCEPT

#include <boost/bind/mem_fn_cc.hpp>

#if defined( __cpp_noexcept_function_type ) || defined( _NOEXCEPT_TYPES_SUPPORTED )
#  undef BOOST_MEM_FN_NOEXCEPT
#  define BOOST_MEM_FN_NOEXCEPT noexcept
#  include <boost/bind/mem_fn_cc.hpp>
#endif

#undef BOOST_MEM_FN_NAME
#undef BOOST_MEM_FN_CC
#undef BOOST_MEM_FN_NOEXCEPT

#if defined(BOOST_MEM_FN_ENABLE_CDECL) && !defined(_M_X64)

#define BOOST_MEM_FN_NAME(X) X##_cdecl
#define BOOST_MEM_FN_CC __cdecl
#define BOOST_MEM_FN_NOEXCEPT

#include <boost/bind/mem_fn_cc.hpp>

#undef BOOST_MEM_FN_NAME
#undef BOOST_MEM_FN_CC
#undef BOOST_MEM_FN_NOEXCEPT

#endif

#if defined(BOOST_MEM_FN_ENABLE_STDCALL) && !defined(_M_X64)

#define BOOST_MEM_FN_NAME(X) X##_stdcall
#define BOOST_MEM_FN_CC __stdcall
#define BOOST_MEM_FN_NOEXCEPT

#include <boost/bind/mem_fn_cc.hpp>

#undef BOOST_MEM_FN_NAME
#undef BOOST_MEM_FN_CC
#undef BOOST_MEM_FN_NOEXCEPT

#endif

#if defined(BOOST_MEM_FN_ENABLE_FASTCALL) && !defined(_M_X64)

#define BOOST_MEM_FN_NAME(X) X##_fastcall
#define BOOST_MEM_FN_CC __fastcall
#define BOOST_MEM_FN_NOEXCEPT

#include <boost/bind/mem_fn_cc.hpp>

#undef BOOST_MEM_FN_NAME
#undef BOOST_MEM_FN_CC
#undef BOOST_MEM_FN_NOEXCEPT

#endif

// data member support

namespace _mfi
{

template<class R, class T> class dm
{
public:

    typedef R const & result_type;
    typedef T const * argument_type;

private:
    
    typedef R (T::*Pm);
    Pm pm_;

public:

    explicit dm( Pm pm ): pm_( pm ) {}

    template<class U,
        class Ud = typename _mfi::remove_cvref<U>::type,
        class En = typename std::enable_if<
            std::is_same<T, Ud>::value || std::is_base_of<T, Ud>::value
        >::type
    >

    auto operator()( U&& u ) const -> decltype( std::forward<U>( u ).*pm_ )
    {
        return std::forward<U>( u ).*pm_;
    }

    template<class U,
        class Ud = typename _mfi::remove_cvref<U>::type,
        class E1 = void,
        class En = typename std::enable_if<
            !(std::is_same<T, Ud>::value || std::is_base_of<T, Ud>::value)
        >::type
    >

    auto operator()( U&& u ) const -> decltype( get_pointer( std::forward<U>( u ) )->*pm_ )
    {
        return get_pointer( std::forward<U>( u ) )->*pm_;
    }

#if BOOST_WORKAROUND(BOOST_MSVC, < 1910)

    template<class U>
    R& operator()( U* u ) const
    {
        return u->*pm_;
    }

    template<class U>
    R const& operator()( U const* u ) const
    {
        return u->*pm_;
    }

#endif

    bool operator==( dm const & rhs ) const
    {
        return pm_ == rhs.pm_;
    }

    bool operator!=( dm const & rhs ) const
    {
        return pm_ != rhs.pm_;
    }
};

} // namespace _mfi

template<class R, class T> _mfi::dm<R, T> mem_fn(R T::*f)
{
    return _mfi::dm<R, T>(f);
}

} // namespace boost

#endif // #ifndef BOOST_BIND_MEM_FN_HPP_INCLUDED
