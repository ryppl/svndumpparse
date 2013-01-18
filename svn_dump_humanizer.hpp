// Copyright Dave Abrahams 2013. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef SVN_DUMP_HUMANIZER_DWA201319_HPP
# define SVN_DUMP_HUMANIZER_DWA201319_HPP
# include "svn_dump_parser.hpp"

namespace ryppl {

// "Proves the concept" of svn_dump_parser by producing a
// human-readable rendition of the dump.  This component will probably
// not be used except for testing.
struct svn_dump_humanizer : svn_dump_parser
{
    svn_dump_humanizer(apr_pool_t* pool)
        : svn_dump_parser(pool), rev_num(-1UL) {}

 private: // virtual function implementations

    //
    // Top-level events
    //
    
    // The parser has discovered a new uuid record
    void uuid_record(const char *uuid, apr_pool_t *pool);

    // The parser has discovered a new revision record
    void begin_revision(apr_hash_t *headers, apr_pool_t *pool);

    // the named property of the current revision to was set to value. 
    void set_revision_property(const char *name, const svn_string_t *value);

    // The parser has reached the end of the current revision
    void end_revision();

    //
    // Events within a given revision
    //
    
    // The parser has discovered a new node (file) record
    void begin_node(apr_hash_t *headers, apr_pool_t *pool);

    // The parser has reached the end of the current node
    void end_node();
    
    //
    // Events within a given node
    //
    // the named property of the current node was set to value
    void set_node_property(const char *name, const svn_string_t *value);
    
    // the named property of the current node was deleted
    void delete_node_property(const char *name);
    
    // all properties of the current node were deleted
    void remove_node_props();

    // The given fulltext provides the next section of the node's contents
    void write_fulltext_stream(const char *data, apr_size_t *len);
    // There are no more fulltext sections in this node
    void close_fulltext_stream();
    
    // The given text delta should be applied against the node's
    // previous contents.
    void apply_textdelta(svn_txdelta_window_t *window);
    
 private: // data members
    // Revision number
    unsigned long rev_num;
};

}

#endif // SVN_DUMP_HUMANIZER_DWA201319_HPP
