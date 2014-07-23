cmake_minimum_required(VERSION 2.6)
project(MERGEBASEINDEXER C)

#SET(CMAKE_C_FLAGS_DISTRIBUTION "-O3")
set(CMAKE_BUILD_TYPE "Debug")
include_directories(.)
add_executable (indexer main.c postingList.c hash.c hashWriter.c stem.c merge.c) 
add_executable (search search.c postingList.c hash.c stem.c merge.c) 
