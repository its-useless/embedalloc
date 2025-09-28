cmake_minimum_required(VERSION 3.20)
project(embedalloc)

include_directories("include")

add_library(embedalloc STATIC "src/embedalloc.c")

add_executable(main "src/main.c")
target_link_libraries(main embedalloc)