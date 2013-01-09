// Copyright Dave Abrahams 2013. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <exception>
#include <iostream>
#include "svn_failure.hpp"
#include "svn_dump_parser.hpp"

#include <svn_repos.h>
#include "svn_error.h"
#include "svn_string.h"
#include "svn_opt.h"
#include "svn_utf.h"
#include "svn_dirent_uri.h"
#include "svn_path.h"
#include "svn_hash.h"
#include "svn_repos.h"
#include "svn_fs.h"
#include "svn_pools.h"
#include "svn_sorts.h"
#include "svn_props.h"
#include "svn_mergeinfo.h"
#include "svn_version.h"


namespace ryppl {

// Adapted from subversion/svndumpfilter/main.c
static svn_stream_t*
create_stdio_stream(
    APR_DECLARE(apr_status_t) open_fn(apr_file_t **, apr_pool_t *),
    apr_pool_t *pool)
{
  apr_file_t *stdio_file;
  apr_status_t apr_err = open_fn(&stdio_file, pool);

  if (apr_err)
      check_svn_failure(svn_error_wrap_apr(apr_err, "Can't open stdio file"));

  return svn_stream_from_aprfile2(stdio_file, TRUE, pool);
}

}

int main()
{
    using namespace ryppl;
    
    // Create our top-level pool.
    apr_allocator_t *allocator;
    if (apr_allocator_create(&allocator))
        return EXIT_FAILURE;

    apr_allocator_max_free_set(allocator, SVN_ALLOCATOR_RECOMMENDED_MAX_FREE);

    apr_pool_t *pool;
    pool = svn_pool_create_ex(NULL, allocator);
    apr_allocator_owner_set(allocator, pool);
    
    try
    {
        check_svn_failure(svn_fs_initialize(pool));
        
        svn_dump_parser parse(pool);
        
        parse(create_stdio_stream(apr_file_open_stdin, pool));
    }
    catch(svn_failure const& x)
    {
        std::cerr << "Failed with svn error: " << x.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch(std::exception const& x)
    {
        std::cerr << "Failed with std::exception: " << x.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch(...)
    {
        std::cerr << "Failed with unknown exception" << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

