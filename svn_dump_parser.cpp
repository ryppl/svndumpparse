// Copyright Dave Abrahams 2013. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "svn_dump_parser.hpp"
#include "svn_failure.hpp"
#include <svn_io.h>
#include <svn_repos.h>
#include <svn_types.h>
#include <svn_error.h>
#include <svn_string.h>
#include <svn_opt.h>
#include <svn_utf.h>
#include <svn_dirent_uri.h>
#include <svn_path.h>
#include <svn_hash.h>
#include <svn_repos.h>
#include <svn_fs.h>
#include <svn_pools.h>
#include <svn_sorts.h>
#include <svn_props.h>
#include <svn_mergeinfo.h>
#include <svn_version.h>

namespace ryppl {

namespace svn_parse_detail {
extern "C"
{
  // The parser has discovered a new revision record within the parsing
  // session represented by parse_baton.
  static svn_error_t* new_revision_record(
      void **revision_baton, apr_hash_t *headers, void *parse_baton, apr_pool_t *pool)
  {
      return 0;
  }

  // The parser has discovered a new uuid record within the parsing
  // session represented by parse_baton.
  static svn_error_t* uuid_record(const char *uuid, void *parse_baton, apr_pool_t *pool)
  {
      return 0;
  }
    
  // The parser has discovered a new node record within the current
  // revision represented by revision_baton.
  static svn_error_t* new_node_record(
      void **node_baton, apr_hash_t *headers, void *revision_baton, apr_pool_t *pool)
  {
      return 0;
  }
    
  // For a given revision_baton, set a property name to value. 
  static svn_error_t* set_revision_property(
      void *revision_baton, const char *name, const svn_string_t *value)
  {
      return 0;
  }
    
  // For a given node_baton, set a property name to value. 
  static svn_error_t* set_node_property(
      void *node_baton, const char *name, const svn_string_t *value)
  {
      return 0;
  }
    
  // For a given node_baton, delete property name. 
  static svn_error_t* delete_node_property(void *node_baton, const char *name)
  {
      return 0;
  }
    
  // For a given node_baton, remove all properties. 
  static svn_error_t* remove_node_props(void *node_baton)
  {
      return 0;
  }
    
  // For a given node_baton, receive a writable stream capable of
  // receiving the node's fulltext.
  static svn_error_t* set_fulltext(svn_stream_t **stream, void *node_baton)
  {
      return 0;
  }
    
  // For a given node_baton, set handler and handler_baton to a window
  // handler and baton capable of receiving a delta against the node's
  // previous contents.
  static svn_error_t* apply_textdelta(
      svn_txdelta_window_handler_t *handler, void **handler_baton, void *node_baton)
  {
      return 0;
  }
    
  // The parser has reached the end of the current node represented by
  // node_baton, it can be freed.
  static svn_error_t* close_node(void *node_baton)
  {
      return 0;
  }
    
  // The parser has reached the end of the current revision
  // represented by revision_baton.
  static svn_error_t* close_revision(void *revision_baton)
  {
      return 0;
  }
}

svn_repos_parse_fns2_t parse_vtable =
{
    new_revision_record,
    uuid_record,
    new_node_record,    
    set_revision_property,    
    set_node_property,    
    delete_node_property,    
    remove_node_props,    
    set_fulltext,    
    apply_textdelta,    
    close_node,    
    close_revision
};
}

svn_dump_parser::svn_dump_parser(apr_pool_t* pool)
    : pool(pool), rev_num(0)
{
}

void svn_dump_parser::operator()(svn_stream_t* in_stream)
{

    check_svn_failure(
        svn_repos_parse_dumpstream2(
            in_stream, &svn_parse_detail::parse_vtable, this, NULL, NULL, pool));
}

}
