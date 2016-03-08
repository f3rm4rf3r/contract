
// no #include guard

// Test error if static inv declared cv (unless PERMISSIVE #defined).

#include <boost/contract/public_function.hpp>
#include <boost/contract/guard.hpp>

struct a {
    void static_invariant() const volatile {}

    void f() {
        // Same for ctor and dtor (because they all use check_pre_post_inv).
        boost::contract::guard c = boost::contract::public_function(this);
    }
};

int main() {
    a aa;
    aa.f();
    return 0;
};

