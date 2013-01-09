// Copyright Dave Abrahams 2013. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef SVN_FAILURE_DWA201319_HPP
# define SVN_FAILURE_DWA201319_HPP

# include <exception>

struct svn_error_t;

namespace ryppl {

struct svn_failure : std::exception
{
    explicit svn_failure(::svn_error_t* err)
        : err(err) {}

    char const* what() const throw();
    
    ::svn_error_t* err;
};

inline void check_svn_failure(::svn_error_t* err)
{
    if (err)
        throw svn_failure(err);
}

svn_error_t* current_exception_to_svn_error();

# define RYPPL_HANDLE_SVN_EXCEPTION(...)                \
    try { __VA_ARGS__ } catch(...) {                    \
        return current_exception_to_svn_error();        \
    } return 0;
}

#endif // SVN_FAILURE_DWA201319_HPP
