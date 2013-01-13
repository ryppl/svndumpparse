svndumpparse
============

A C++ library providing a base class `svn_dump_parser` that uses
SVN's own code to parse and process an SVN dump file.

The project contains a test program that subclasses `svn_dump_parser`
and uses it to produce a human-readable version of the log.
