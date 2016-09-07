
#ifndef BOOST_CONTRACT_ACCESS_HPP_
#define BOOST_CONTRACT_ACCESS_HPP_

// Copyright (C) 2008-2016 Lorenzo Caminiti
// Distributed under the Boost Software License, Version 1.0 (see accompanying
// file LICENSE_1_0.txt or a copy at http://www.boost.org/LICENSE_1_0.txt).
// See: http://www.boost.org/doc/libs/release/libs/contract/doc/html/index.html

/** @file
Facility to declare invariants, base types, etc all as private members.
*/

#include <boost/contract/core/config.hpp>
#include <boost/contract/detail/decl.hpp>
#if     !defined(BOOST_CONTRACT_NO_INVARIANTS) || \
        !defined(BOOST_CONTRACT_NO_PRECONDITIONS) || \
        !defined(BOOST_CONTRACT_NO_POSTCONDITIONS) || \
        !defined(BOOST_CONTRACT_NO_EXCEPTS)
    #include <boost/contract/detail/type_traits/introspection.hpp>
#endif
#ifndef BOOST_CONTRACT_NO_INVARIANTS
    #include <boost/contract/detail/debug.hpp>
    #include <boost/function_types/property_tags.hpp>
    #include <boost/mpl/vector.hpp>
#endif
#include <boost/noncopyable.hpp>

namespace boost {
    namespace contract {
        class virtual_;

        namespace detail {
            BOOST_CONTRACT_DETAIL_DECL_DETAIL_COND_WITH_SUBCONTRACTING_Z(1,
                /* is_friend = */ 0, OO, RR, FF, CC, AArgs);
            
            template<typename RR, class CC>
            class cond_with_inv;
        }
    }
}

namespace boost { namespace contract {

/**
Friend this class to declare invariants and base types as private members.
Declare this class as friend of the contracted class in order to declare
the invariants functions and the base types @c typedef as non-public members.
In real code, programmers will likely chose to declare this class as friend so
to fully control the contracted class public interface.

This class is not intended to be directly used by programmers a part from
declaring it @c friend (and that is why this class does not have any public
member and it is not copyable).

@note   Not making this class friend of the contracted class will cause
        compiler errors on some compilers (e.g., MSVC) because the private
        members needed to check the contracts will not be accessible. On other
        compilers (e.g., GCC and CLang), the private access will instead fail
        SFINAE and no compiler error will be reported while invariants and
        subcontracting will be silently skipped at run-time. Therefore,
        programmers must make sure to either declare invariant functions and
        base types @c typedef as public members or to make this class a friend.
@see @RefSect{advanced_topics, Advanced Topics}
*/
class access : private boost::noncopyable {
/** @cond */
    // No public APIs (so users cannot use it directly by mistake).

    #if     !defined(BOOST_CONTRACT_NO_INVARIANTS) || \
            !defined(BOOST_CONTRACT_NO_PRECONDITIONS) || \
            !defined(BOOST_CONTRACT_NO_POSTCONDITIONS) || \
            !defined(BOOST_CONTRACT_NO_EXCEPTS)
        BOOST_CONTRACT_DETAIL_INTROSPECTION_HAS_TYPE(has_base_types,
                BOOST_CONTRACT_BASE_TYPEDEF)

        template<class C>
        struct base_types_of {
            typedef typename C::BOOST_CONTRACT_BASE_TYPEDEF type;
        };
    #endif
    
    #ifndef BOOST_CONTRACT_NO_INVARIANTS
        BOOST_CONTRACT_DETAIL_INTROSPECTION_HAS_MEMBER_FUNCTION(
                has_static_invariant_f, BOOST_CONTRACT_STATIC_INVARIANT)
        
        BOOST_CONTRACT_DETAIL_INTROSPECTION_HAS_STATIC_MEMBER_FUNCTION(
                has_static_invariant_s, BOOST_CONTRACT_STATIC_INVARIANT)

        template<class C>
        struct has_static_invariant : has_static_invariant_s<C, void,
                boost::mpl::vector<> > {};

        template<class C>
        static void static_invariant() {
            C::BOOST_CONTRACT_STATIC_INVARIANT();
        }

        template<class C>
        class static_invariant_addr { // Class so to pass it as tparam.
            typedef void (*func_ptr)();
        public:
            static func_ptr apply() {
                return &C::BOOST_CONTRACT_STATIC_INVARIANT;
            }
        };

        BOOST_CONTRACT_DETAIL_INTROSPECTION_HAS_MEMBER_FUNCTION(
                has_invariant_f, BOOST_CONTRACT_INVARIANT)
        
        BOOST_CONTRACT_DETAIL_INTROSPECTION_HAS_STATIC_MEMBER_FUNCTION(
                has_invariant_s, BOOST_CONTRACT_INVARIANT)

        template<class C>
        struct has_cv_invariant : has_invariant_f<C, void, boost::mpl::vector<>,
                boost::function_types::cv_qualified> {};
        
        template<class C>
        struct has_const_invariant : has_invariant_f<C, void, boost::mpl::
                vector<>, boost::function_types::const_qualified> {};

        template<class C>
        static void cv_invariant(C const volatile* obj) {
            BOOST_CONTRACT_DETAIL_DEBUG(obj);
            obj->BOOST_CONTRACT_INVARIANT();
        }
        
        template<class C>
        static void const_invariant(C const* obj) {
            BOOST_CONTRACT_DETAIL_DEBUG(obj);
            obj->BOOST_CONTRACT_INVARIANT();
        }
    #endif
    
    // Friends (used to limit library's public API).

    // NOTE: Using friends here and in all other places in this library does not
    // increase compilation times (I experimented replacing all friends with
    // public and got the same compilation times).

    BOOST_CONTRACT_DETAIL_DECL_DETAIL_COND_WITH_SUBCONTRACTING_Z(1,
            /* is_friend = */ 1, OO, RR, FF, CC, AArgs);
            
    template<typename RR, class CC>
    friend class boost::contract::detail::cond_with_inv;
        
    BOOST_CONTRACT_DETAIL_DECL_FRIEND_OVERRIDING_PUBLIC_FUNCTIONS_Z(1,
            OO, RR, FF, CC, AArgs, vv, rr, ff, oobj, aargs)
/** @endcond */
};

} } // namespace

#endif // #include guard

