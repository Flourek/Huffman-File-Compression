cmake_minimum_required(VERSION 3.16.3)
project(ZipProject C)

set(CMAKE_C_STANDARD 99)


add_executable(ZipProject main.c helper.c header.h huffman.c zip.c unzip.c)




