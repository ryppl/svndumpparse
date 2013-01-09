// Copyright Dave Abrahams 2013. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef SVN_DUMP_HUMANIZER_DWA201319_HPP
# define SVN_DUMP_HUMANIZER_DWA201319_HPP
# include "svn_dump_parser.hpp"

namespace ryppl {

struct svn_dump_humanizer : svn_dump_parser
{
    svn_dump_humanizer(apr_pool_t* pool)
        : svn_dump_parser(pool), rev_num(-1UL) {}

 private: // virtual function implementations
    // The parser has discovered a new revision record
    void begin_revision(apr_hash_t *headers, apr_pool_t *pool);

    // The parser has discovered a new uuid record
    void uuid_record(const char *uuid, apr_pool_t *pool);

    // The parser has discovered a new node record within the current
    // revision represented by revision_baton.
    void begin_node(apr_hash_t *headers, apr_pool_t *pool);
    
    // set a named property of the current revision to value. 
    void set_revision_property(const char *name, const svn_string_t *value);
    
    // set a named property of the current node to value. 
    void set_node_property(const char *name, const svn_string_t *value);
    
    // delete a named property of the current node
    void delete_node_property(const char *name);
    
    // remove all properties of the current node.
    void remove_node_props();
    
    void write_fulltext_stream(const char *data, apr_size_t *len);
    void close_fulltext_stream();
    
    // For a given node_baton, set handler and handler_baton to a window
    // handler and baton capable of receiving a delta against the node's
    // previous contents.
    void apply_textdelta(svn_txdelta_window_t *window);
    
    // The parser has reached the end of the current node
    void end_node();
    
    // The parser has reached the end of the current revision
    void end_revision();
    
 private: // data members
    // Revision number
    unsigned long rev_num;
};

}

#endif // SVN_DUMP_HUMANIZER_DWA201319_HPP
