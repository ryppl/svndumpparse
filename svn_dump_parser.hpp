// Copyright Dave Abrahams 2013. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef SVN_DUMP_PARSER_DWA201319_HPP
# define SVN_DUMP_PARSER_DWA201319_HPP

# include <apr.h>

struct svn_stream_t;
struct apr_pool_t;
struct apr_hash_t;
struct svn_string_t;
struct svn_txdelta_window_t;

namespace ryppl {

// An abstract base class for general subversion dump parsing.  It's
// basically a visitor for the subversion dump format.
struct svn_dump_parser
{
    svn_dump_parser(apr_pool_t* pool);
    void operator()(svn_stream_t* in_stream);

    class backdoor;
 private: // virtual functions
    //
    // Top-level events
    //    
    // The parser has discovered a new uuid record
    virtual void uuid_record(const char *uuid, apr_pool_t *pool) = 0;

    // The parser has discovered a new revision record
    virtual void begin_revision(apr_hash_t *headers, apr_pool_t *pool) = 0;

    // the named property of the current revision to was set to value. 
    virtual void set_revision_property(const char *name, const svn_string_t *value) = 0;

    // The parser has reached the end of the current revision
    virtual void end_revision() = 0;

    //
    // Events within a given revision
    //    
    // The parser has discovered a new node (file) record
    virtual void begin_node(apr_hash_t *headers, apr_pool_t *pool) = 0;

    // The parser has reached the end of the current node
    virtual void end_node() = 0;
    
    //
    // Events within a given node
    //
    // the named property of the current node was set to value
    virtual void set_node_property(const char *name, const svn_string_t *value) = 0;
    
    // the named property of the current node was deleted
    virtual void delete_node_property(const char *name) = 0;
    
    // all properties of the current node were deleted
    virtual void remove_node_props() = 0;

    // The given fulltext provides the next section of the node's contents
    virtual void write_fulltext_stream(const char *data, apr_size_t *len) = 0;
    // There are no more fulltext sections in this node
    virtual void close_fulltext_stream() = 0;
    
    // The given text delta should be applied against the node's
    // previous contents.
    virtual void apply_textdelta(svn_txdelta_window_t *window) = 0;
    
 private:
    apr_pool_t* pool;
};

}

#endif // SVN_DUMP_PARSER_DWA201319_HPP
