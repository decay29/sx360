cmake_minimum_required(VERSION 3.6)
project (sx360)

set(SOURCE_FILES sxiterator.cpp sxpunchfile.cpp sxrecord.cpp)
add_library(sx360 STATIC ${SOURCE_FILES})
