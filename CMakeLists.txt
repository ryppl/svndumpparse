cmake_minimum_required(VERSION 2.8)

# include(FindSubversionLibrary)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

find_package(SubversionLibrary 1.78)
find_package(APR)

include_directories(${SUBVERSION_INCLUDE_DIRS} ${APR_INCLUDE_DIRS} ${GETTEXT_INCLUDE_DIR})

add_executable(convert
  main.cpp)

target_link_libraries(convert ${SUBVERSION_LIBRARIES} ${APR_LIBRARY} ${GETTEXT_LIBRARIES})


