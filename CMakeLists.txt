cmake_minimum_required(VERSION 2.8)

list(APPEND CMAKE_MODULE_PATH 
  ${CMAKE_CURRENT_LIST_DIR}/cmake 
  ${CMAKE_CURRENT_LIST_DIR}/cmake/cmake-cxx11/Modules
  )
include(CheckCXX11Features)

find_package(SubversionLibrary 1.78)
find_package(APR)
find_package(Boost COMPONENTS system filesystem  REQUIRED)

set(SUBCONVERT_SRC ${CMAKE_CURRENT_LIST_DIR}/subconvert/src)

include_directories(
  ${CMAKE_CURRENT_LIST_DIR} 
  ${CMAKE_CURRENT_LIST_DIR}/libgit2/include
  ${SUBVERSION_INCLUDE_DIRS} 
  ${APR_INCLUDE_DIRS} 
  ${SUBCONVERT_SRC}
  ${Boost_INCLUDE_DIRS}
  )

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  list(APPEND CMAKE_CXX_FLAGS "-std=c++11 -Werror")
endif()

add_executable(modhist
  main.cpp
  svn_failure.cpp
  svn_dump_parser.cpp
  svn_dump_humanizer.cpp
)

include_directories(${SUBVERSION_INCLUDE_DIRS} ${APR_INCLUDE_DIRS} subconvert/src/)

add_library(subconvert
  ${SUBCONVERT_SRC}/authors.cpp
  ${SUBCONVERT_SRC}/branches.cpp
  ${SUBCONVERT_SRC}/converter.cpp
  ${SUBCONVERT_SRC}/gitutil.cpp
  ${SUBCONVERT_SRC}/authors.cpp
)

target_link_libraries(
  subconvert
  git2
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY})

add_subdirectory(libgit2)


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set_property(TARGET git2
    PROPERTY COMPILE_FLAGS "-Wno-unused-value -Wno-sizeof-pointer-memaccess"
    )
endif()

foreach(flag ${CXX11_FEATURE_LIST})
  set_property(TARGET modhist
	APPEND PROPERTY COMPILE_DEFINITIONS ${flag}
	)
endforeach()

target_link_libraries(
  modhist subconvert
#  ${Boost_FILESYSTEM_LIBRARY}
#  ${Boost_SYSTEM_LIBRARY}
  ${SUBVERSION_LIBRARIES} ${APR_LIBRARY})



