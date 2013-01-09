// Copyright Dave Abrahams 2013. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include "svn_failure.hpp"

# include <svn_types.h>
# include <svn_error.h>

namespace ryppl {

char const* svn_failure::what() const throw()
{
    return err->message;
}

svn_error_t* current_exception_to_svn_error()
{
    try {
        throw;
    }
    catch(svn_failure const& x)
    {
        return x.err;
    }
    catch(std::exception& e)
    {
        return svn_error_createf(
            APR_OS_START_USERERR, NULL, "Unknown std::exception: %s", e.what());
    }
    catch(...)
    {
        return svn_error_createf(
            APR_OS_START_USERERR, NULL, "Unknown exception");
    }
}

}
