cmake_minimum_required(VERSION 2.8)

list(APPEND CMAKE_MODULE_PATH 
  ${CMAKE_CURRENT_LIST_DIR}/cmake 
  )

find_package(SubversionLibrary 1.78)
find_package(APR)

include_directories(
  ${CMAKE_CURRENT_LIST_DIR} 
  ${SUBVERSION_INCLUDE_DIRS}
  ${APR_INCLUDE_DIRS} 
  )

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  list(APPEND CMAKE_CXX_FLAGS "-Werror")
endif()

add_library(svndumpparse
  svn_failure.cpp
  svn_dump_parser.cpp
)

add_executable(test
  main.cpp
  svn_dump_humanizer.cpp
)

include_directories(${SUBVERSION_INCLUDE_DIRS} ${APR_INCLUDE_DIRS})

target_link_libraries(
  test
  svndumpparse
  ${SUBVERSION_LIBRARIES} ${APR_LIBRARY})



