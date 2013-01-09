// Copyright Dave Abrahams 2013. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include "svn_dump_humanizer.hpp"

#include <iostream>
#include <string>
#include <svn_string.h>
#include <apr_hash.h>

namespace ryppl {

static void dump_headers(apr_pool_t* pool, apr_hash_t* headers, char const* prefix)
{
    for (apr_hash_index_t* i = apr_hash_first(pool, headers); i; i = apr_hash_next(i))
    {
        char const* key;
        char const* val;
        apr_hash_this(i, (const void**)&key, NULL, (void**)&val);
        
        std::cout << prefix << "( " << key << ": " << val << ")" << std::endl;
    }
}

// The parser has discovered a new revision record
void svn_dump_humanizer::begin_revision(apr_hash_t *headers, apr_pool_t *pool)
{
    ++rev_num;
    std::cout << "{ revision: " << rev_num << std::endl;
    dump_headers(pool, headers, "  ");
}

// The parser has discovered a new uuid record
void svn_dump_humanizer::uuid_record(const char *uuid, apr_pool_t *pool)
{
    std::cout << "UUID " << uuid << std::endl;
}

// The parser has discovered a new node record within the current
// revision represented by revision_baton.
void svn_dump_humanizer::begin_node(apr_hash_t *headers, apr_pool_t *pool)
{
    std::cout << "  { node " << std::endl;
    dump_headers(pool, headers, "    ");
}
    
// set a named property of the current revision to value. 
void svn_dump_humanizer::set_revision_property(const char *name, const svn_string_t *value)
{
    std::cout << "  [ set-revision-property "
              << name << "=" << std::string(value->data, value->data + value->len)
              << " ]" << std::endl;
}
    
// set a named property of the current node to value. 
void svn_dump_humanizer::set_node_property(const char *name, const svn_string_t *value)
{
    std::cout << "    [ set-node-property "
              << name << "=" << std::string(value->data, value->data + value->len)
              << " ]" << std::endl;
}
    
// delete a named property of the current node
void svn_dump_humanizer::delete_node_property(const char *name)
{
    std::cout << "    [ delete-node-property "
              << name
              << " ]" << std::endl;
    
}
    
// remove all properties of the current node.
void svn_dump_humanizer::remove_node_props()
{
    std::cout << "    [ delete-all-node-properties "
              << " ]" << std::endl;
}
    
void svn_dump_humanizer::write_fulltext_stream(const char *data, apr_size_t *len)
{
    std::cout << "<fulltext: " << *len << "> ";
}

void svn_dump_humanizer::close_fulltext_stream()
{
    std::cout << "<EOS>" << std::endl;
}

void svn_dump_humanizer::apply_textdelta(svn_txdelta_window_t *window)
{
    std::cout << "    [ textdelta "
              << " ]" << std::endl;
    
}
    
// The parser has reached the end of the current node
void svn_dump_humanizer::end_node()
{
    std::cout << "  }" << std::endl;
}
    
// The parser has reached the end of the current revision
void svn_dump_humanizer::end_revision()
{
    std::cout << "}" << std::endl;
}

}
