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

extern "C"
{
  svn_error_t *text_stream_writer(
      void *baton, const char *data, apr_size_t *len);
  svn_error_t *text_stream_closer(void *baton);
}

struct svn_dump_parser::backdoor
{
    // The parser has discovered a new revision record
    static void begin_revision(svn_dump_parser* parser, apr_hash_t *headers, apr_pool_t *pool)
    {
        parser->begin_revision(headers, pool);
    }

    // The parser has discovered a new uuid record
    static void uuid_record(svn_dump_parser* parser, const char *uuid, apr_pool_t *pool)
    {
        parser->uuid_record(uuid, pool);
    }

    // The parser has discovered a new node record within the current
    // revision represented by revision_baton.
    static void begin_node(svn_dump_parser* parser, apr_hash_t *headers, apr_pool_t *pool)
    {
        parser->begin_node(headers, pool);
    }
    
    // set a named property of the current revision to value. 
    static void set_revision_property(svn_dump_parser* parser, const char *name, const svn_string_t *value)
    {
        parser->set_revision_property(name, value);
    }
    
    // set a named property of the current node to value. 
    static void set_node_property(svn_dump_parser* parser, const char *name, const svn_string_t *value)
    {
        parser->set_node_property(name, value);
    }
    
    // delete a named property of the current node
    static void delete_node_property(svn_dump_parser* parser, const char *name)
    {
        parser->delete_node_property(name);
    }
    
    // remove all properties of the current node.
    static void remove_node_props(svn_dump_parser* parser)
    {
        parser->remove_node_props();
    }
    
    // receive a writable stream capable of receiving the current
    // node's fulltext.
    static void set_fulltext(svn_dump_parser* parser, svn_stream_t **stream)
    {
        *stream = svn_stream_create(parser, parser->pool);
        svn_stream_set_write(*stream, text_stream_writer);
        svn_stream_set_close(*stream, text_stream_closer);
    }
    
    // For a given node_baton, set handler and handler_baton to a window
    // handler and baton capable of receiving a delta against the node's
    // previous contents.
    static void apply_textdelta(svn_dump_parser* parser, svn_txdelta_window_t *window)
    {
        parser->apply_textdelta(window);
    }
    
    // The parser has reached the end of the current node
    static void end_node(svn_dump_parser* parser)
    {
        parser->end_node();
    }
    
    // The parser has reached the end of the current revision
    static void end_revision(svn_dump_parser* parser)
    {
        parser->end_revision();
    }

    static void write_fulltext_stream(svn_dump_parser* parser, const char *data, apr_size_t *len)
    {
        parser->write_fulltext_stream(data, len);
    }
    
    static void close_fulltext_stream(svn_dump_parser* parser)
    {
        parser->close_fulltext_stream();
    }
    
};

namespace svn_parse_detail {

extern "C"
{
  // The parser has discovered a new revision record within the parsing
  // session represented by parse_baton.
  static svn_error_t* new_revision_record(
      void **revision_baton, apr_hash_t *headers, void *parse_baton, apr_pool_t *pool)
  {
      RYPPL_HANDLE_SVN_EXCEPTION(
          svn_dump_parser* parser = static_cast<svn_dump_parser*>(parse_baton);
          *revision_baton = parser;
          svn_dump_parser::backdoor::begin_revision(parser, headers, pool);
      )
  }

  // The parser has discovered a new uuid record within the parsing
  // session represented by parse_baton.
  static svn_error_t* uuid_record(const char *uuid, void *parse_baton, apr_pool_t *pool)
  {
      RYPPL_HANDLE_SVN_EXCEPTION(
          svn_dump_parser* parser = static_cast<svn_dump_parser*>(parse_baton);
          svn_dump_parser::backdoor::uuid_record(parser, uuid, pool);
      )
  }
    
  // The parser has discovered a new node record within the current
  // revision represented by revision_baton.
  static svn_error_t* new_node_record(
      void **node_baton, apr_hash_t *headers, void *revision_baton, apr_pool_t *pool)
  {
      RYPPL_HANDLE_SVN_EXCEPTION(
          svn_dump_parser* parser = static_cast<svn_dump_parser*>(revision_baton);
          *node_baton = parser;
          svn_dump_parser::backdoor::begin_node(parser, headers, pool);
      )
  }
    
  // For a given revision_baton, set a property name to value. 
  static svn_error_t* set_revision_property(
      void *revision_baton, const char *name, const svn_string_t *value)
  {
      RYPPL_HANDLE_SVN_EXCEPTION(
          svn_dump_parser* parser = static_cast<svn_dump_parser*>(revision_baton);
          svn_dump_parser::backdoor::set_revision_property(parser, name, value);
      )
  }
    
  // For a given node_baton, set a property name to value. 
  static svn_error_t* set_node_property(
      void *node_baton, const char *name, const svn_string_t *value)
  {
      RYPPL_HANDLE_SVN_EXCEPTION(
          svn_dump_parser* parser = static_cast<svn_dump_parser*>(node_baton);
          svn_dump_parser::backdoor::set_node_property(parser, name, value);
      )
  }
    
  // For a given node_baton, delete property name. 
  static svn_error_t* delete_node_property(void *node_baton, const char *name)
  {
      RYPPL_HANDLE_SVN_EXCEPTION(
          svn_dump_parser* parser = static_cast<svn_dump_parser*>(node_baton);
          svn_dump_parser::backdoor::delete_node_property(parser, name);
      )
  }
    
  // For a given node_baton, remove all properties. 
  static svn_error_t* remove_node_props(void *node_baton)
  {
      RYPPL_HANDLE_SVN_EXCEPTION(
          svn_dump_parser* parser = static_cast<svn_dump_parser*>(node_baton);
          svn_dump_parser::backdoor::remove_node_props(parser);
      )
  }
    
  // For a given node_baton, receive a writable stream capable of
  // receiving the node's fulltext.
  static svn_error_t* set_fulltext(svn_stream_t **stream, void *node_baton)
  {
      RYPPL_HANDLE_SVN_EXCEPTION(
          svn_dump_parser* parser = static_cast<svn_dump_parser*>(node_baton);
          svn_dump_parser::backdoor::set_fulltext(parser, stream);
      )
  }

  // Text delta window handler
  static svn_error_t *txdelta_window_handler(
      svn_txdelta_window_t *window, void *baton)
  {
      RYPPL_HANDLE_SVN_EXCEPTION(
          svn_dump_parser* parser = static_cast<svn_dump_parser*>(baton);
          svn_dump_parser::backdoor::apply_textdelta(parser, window);
      )
  }

  // For a given node_baton, set handler and handler_baton to a window
  // handler and baton capable of receiving a delta against the node's
  // previous contents.
  static svn_error_t* apply_textdelta(
      svn_txdelta_window_handler_t *handler, void **handler_baton, void *node_baton)
  {
      *handler = txdelta_window_handler;
      *handler_baton = node_baton;
      return 0;
  }
    
  // The parser has reached the end of the current node represented by
  // node_baton, it can be freed.
  static svn_error_t* close_node(void *node_baton)
  {
      RYPPL_HANDLE_SVN_EXCEPTION(
          svn_dump_parser* parser = static_cast<svn_dump_parser*>(node_baton);
          svn_dump_parser::backdoor::end_node(parser);
      )
  }
    
  // The parser has reached the end of the current revision
  // represented by revision_baton.
  static svn_error_t* close_revision(void *revision_baton)
  {
      RYPPL_HANDLE_SVN_EXCEPTION(
          svn_dump_parser* parser = static_cast<svn_dump_parser*>(revision_baton);
          svn_dump_parser::backdoor::end_revision(parser);
      )
  }

  svn_error_t *text_stream_writer(
      void *baton, const char *data, apr_size_t *len)
  {
      RYPPL_HANDLE_SVN_EXCEPTION(
          svn_dump_parser* parser = static_cast<svn_dump_parser*>(baton);
          svn_dump_parser::backdoor::write_fulltext_stream(parser, data, len);
      )
  }


  // Close function for the publically returned stream. */
  svn_error_t *
  text_stream_closer(void *baton)
  {
      RYPPL_HANDLE_SVN_EXCEPTION(
          svn_dump_parser* parser = static_cast<svn_dump_parser*>(baton);
          svn_dump_parser::backdoor::close_fulltext_stream(parser);
      )
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
    : pool(pool)
{
}

void svn_dump_parser::operator()(svn_stream_t* in_stream)
{
    check_svn_failure(
        svn_repos_parse_dumpstream2(
            in_stream, &svn_parse_detail::parse_vtable, this, NULL, NULL, pool));
}

}
