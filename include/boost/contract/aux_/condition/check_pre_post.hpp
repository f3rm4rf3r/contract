
#ifndef BOOST_CONTRACT_AUX_CHECK_PRE_POST_HPP_
#define BOOST_CONTRACT_AUX_CHECK_PRE_POST_HPP_

// TODO: Fix all header files.
#include <boost/contract/core/config.hpp>
#if BOOST_CONTRACT_POSTCONDITIONS
#   include <boost/contract/core/exception.hpp>
#endif
#include <boost/contract/aux_/condition/check_base.hpp>
#include <boost/contract/aux_/type_traits/optional.hpp>
#include <boost/contract/aux_/none.hpp>
#include <boost/optional.hpp>
#if BOOST_CONTRACT_POSTCONDITIONS
#   include <boost/function.hpp>
#endif
#include <boost/type_traits/remove_reference.hpp>
#include <boost/mpl/if.hpp>
#include <boost/preprocessor/control/expr_iif.hpp>

/* PRIVATE */

#define BOOST_CONTRACT_AUX_CHECK_PRE_POST_DEF_( \
        result_type, result_param, ftor_type, ftor_var, ftor_call) \
    public: \
        template<typename F> \
        void set_post(F const& f) { ftor_var = f; } \
    \
    protected: \
        void check_post(result_type const& result_param) { \
            if(failed()) return; \
            try { if(ftor_var) { ftor_call; } } \
            catch(...) { fail(&boost::contract::postcondition_failure); } \
        } \
    \
    private: \
        boost::function<ftor_type> ftor_var; /* Boost.Func for lambdas, etc. */

/* CODE */

namespace boost { namespace contract { namespace aux {

template<typename R>
class check_pre_post : public check_base { // Non-copyable base.
public:
    explicit check_pre_post(boost::contract::from from) : check_base(from) {}
    
    #if BOOST_CONTRACT_POSTCONDITIONS
        private:
            typedef typename boost::mpl::if_<is_optional<R>,
                boost::optional<typename boost::remove_reference<typename
                        optional_value_type<R>::type>::type const&> const&
            ,
                R const&
            >::type r_type;

            BOOST_CONTRACT_AUX_CHECK_PRE_POST_DEF_(
                r_type,
                r,
                void (r_type),
                // Won't raise this error if NO_POST (for optimization).
                BOOST_CONTRACT_ERROR_postcondition_result_parameter_required,
                BOOST_CONTRACT_ERROR_postcondition_result_parameter_required(r)
            )
    #endif
};

template<>
class check_pre_post<none> : public check_base { // Non-copyable base.
public:
    explicit check_pre_post(boost::contract::from from) : check_base(from) {}
    
    #if BOOST_CONTRACT_POSTCONDITIONS
        BOOST_CONTRACT_AUX_CHECK_PRE_POST_DEF_(
            none,
            unused,
            void (),
            // Won't raise this error if NO_POST (for optimization).
            BOOST_CONTRACT_ERROR_postcondition_result_parameter_not_allowed,
            BOOST_CONTRACT_ERROR_postcondition_result_parameter_not_allowed()
        )
    #endif
};

} } } // namespace

#endif // #include guard

