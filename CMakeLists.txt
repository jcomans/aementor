cmake_minimum_required(VERSION 3.16)

project(aementor)

find_package(CURL)

find_package(Boost)

add_executable(simple
  src/main.cpp
  src/RESTClient.cpp
  )

target_compile_features(simple
  PRIVATE
  cxx_std_17
  )

target_compile_options(simple
  PRIVATE
  -Wall
  )

target_include_directories(simple
  PRIVATE
  ${CMAKE_SOURCE_DIR}/externals
  )

target_link_libraries(simple
  PRIVATE
  CURL::libcurl
  Boost::headers
  )
