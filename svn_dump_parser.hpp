// Copyright Dave Abrahams 2013. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef SVN_DUMP_PARSER_DWA201319_HPP
# define SVN_DUMP_PARSER_DWA201319_HPP

struct svn_stream_t;
struct apr_pool_t;

namespace ryppl {

struct svn_dump_parser
{
    svn_dump_parser(apr_pool_t* pool);
    void operator()(svn_stream_t* in_stream);
        
    apr_pool_t* pool;
    
    // Revision number
    unsigned rev_num;
};

}

#endif // SVN_DUMP_PARSER_DWA201319_HPP
