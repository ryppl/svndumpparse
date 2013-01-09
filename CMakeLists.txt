cmake_minimum_required(VERSION 2.8)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/modules)

find_package(SubversionLibrary 1.78)
find_package(APR)

include_directories(${SUBVERSION_INCLUDE_DIRS} ${APR_INCLUDE_DIRS})

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  list(APPEND CMAKE_CXX_FLAGS "-std=c++11")
endif()

add_executable(modhist
  main.cpp
  svn_failure.cpp
  svn_dump_parser.cpp
)

foreach(flag ${CXX11_FEATURE_LIST})
  set_property(TARGET modhist
	APPEND PROPERTY COMPILE_DEFINITIONS ${flag}
	)
endforeach()

target_link_libraries(modhist ${SUBVERSION_LIBRARIES} ${APR_LIBRARY})


