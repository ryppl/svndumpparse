// Copyright Dave Abrahams 2013. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <exception>
#include <iostream>

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

struct svn_error : std::exception
{
    explicit svn_error(::svn_error_t* err)
        : err(err) {}

    char const* what() const throw()
    {
        return err->message;
    }
    
    ::svn_error_t* err;
};

inline void check_svn_error(::svn_error_t* err)
{
    if (err)
        throw svn_error(err);
}

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

// Lifted out of subversion/svndumpfilter/main.c
static svn_error_t*
create_stdio_stream(
    svn_stream_t **stream,
    APR_DECLARE(apr_status_t) open_fn(apr_file_t **, apr_pool_t *),
    apr_pool_t *pool)
{
  apr_file_t *stdio_file;
  apr_status_t apr_err = open_fn(&stdio_file, pool);

  if (apr_err)
    return svn_error_wrap_apr(apr_err, "Can't open stdio file");

  *stream = svn_stream_from_aprfile2(stdio_file, TRUE, pool);
  return SVN_NO_ERROR;
}

struct parser
{
    parser(apr_pool_t* pool)
        : pool(pool), in_stream(0), out_stream(0), rev_num(0)
    {
        /* Read the stream from STDIN.  Users can redirect a file. */
        check_svn_error(
            create_stdio_stream(&(this->in_stream), apr_file_open_stdin, pool));

        /* Have the parser dump results to STDOUT. Users can redirect a file. */
        check_svn_error(
            create_stdio_stream(&(this->out_stream), apr_file_open_stdout, pool));
    }

    apr_pool_t* pool;
    
    // Input and output streams.
    svn_stream_t* in_stream;
    svn_stream_t* out_stream;

    // Revision number
    unsigned rev_num;
};

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

    if (svn_error_t* err = svn_fs_initialize(pool))
        return EXIT_FAILURE;

    try
    {
        parser parser(pool);
    
        check_svn_error(
            svn_repos_parse_dumpstream2(
                parser.in_stream, &parse_vtable, &parser, NULL, NULL, pool));
    }
    catch(svn_error const& x)
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

