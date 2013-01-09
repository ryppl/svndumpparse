// Copyright Dave Abrahams 2013. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef SVN_DUMP_PARSER_DWA201319_HPP
# define SVN_DUMP_PARSER_DWA201319_HPP

struct svn_stream_t;
struct apr_pool_t;
struct apr_hash_t;
struct svn_string_t;
struct svn_txdelta_window_t;

namespace ryppl {

struct svn_dump_parser
{
    svn_dump_parser(apr_pool_t* pool);
    void operator()(svn_stream_t* in_stream);

    class backdoor;
 private: // virtual functions
    friend class backdoor;
    
    // The parser has discovered a new revision record
    virtual void begin_revision(apr_hash_t *headers, apr_pool_t *pool) = 0;

    // The parser has discovered a new uuid record
    virtual void uuid_record(const char *uuid, apr_pool_t *pool) = 0;

    // The parser has discovered a new node record within the current
    // revision represented by revision_baton.
    virtual void begin_node(apr_hash_t *headers, apr_pool_t *pool) = 0;
    
    // set a named property of the current revision to value. 
    virtual void set_revision_property(const char *name, const svn_string_t *value) = 0;
    
    // set a named property of the current node to value. 
    virtual void set_node_property(const char *name, const svn_string_t *value) = 0;
    
    // delete a named property of the current node
    virtual void delete_node_property(const char *name) = 0;
    
    // remove all properties of the current node.
    virtual void remove_node_props() = 0;
    
    // receive a writable stream capable of receiving the current
    // node's fulltext.
    virtual void set_fulltext(svn_stream_t **stream) = 0;
    
    // For a given node_baton, set handler and handler_baton to a window
    // handler and baton capable of receiving a delta against the node's
    // previous contents.
    virtual void apply_textdelta(svn_txdelta_window_t *window) = 0;
    
    // The parser has reached the end of the current node
    virtual void end_node() = 0;
    
    // The parser has reached the end of the current revision
    virtual void end_revision() = 0;
    
 private:
    apr_pool_t* pool;
};

}

#endif // SVN_DUMP_PARSER_DWA201319_HPP
