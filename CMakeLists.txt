cmake_minimum_required(VERSION 2.8)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/modules)

find_package(SubversionLibrary 1.78)
find_package(APR)

include_directories(${SUBVERSION_INCLUDE_DIRS} ${APR_INCLUDE_DIRS})

add_executable(convert
  main.cpp)

target_link_libraries(convert ${SUBVERSION_LIBRARIES} ${APR_LIBRARY})


