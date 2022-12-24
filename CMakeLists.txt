cmake_minimum_required(VERSION 3.8)
project(morse-binary-tree
        LANGUAGES C
        VERSION 2.7.3
        DESCRIPTION "Using a binary tree to translate Morse code into Ascii text.")

add_executable(morse-binary-tree
    src/morse-binary-tree.c
)