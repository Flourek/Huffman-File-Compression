# Huffman-File-Compression

Simple program that uses Huffman Coding to compress any type of file. It uses canonical Huffman codes to efficiently reconstruct the tree.

> ![Screenshot](images/screenshot.png?raw=true "Title")

# Usage
Simply drag any file over the executable to compress it or launch it from the terminal providing the path of the file as an argument. It works the exact same way for decompressing files. 

# EPI File specification
```c
    char extension[8];       // the first 8 bytes store the extension of the original file 
    int codes_map_len;       // stores how many "CodeBlocks" to read   
    CodeBlock *codes_map;    // stores the huffman codes as SYMBOL | CODE LENGTH to reconstruct the tree
    int last_block_offset;   // the last "DataBlock" has a surplus of bits that aren't codes
    DataBlock *data_blocks;  // packs the otherwise string of binary codes into a 32 bit integer  
```

# Requirements
- Windows Only
- C99

# Compilation
- CMAKE 3.16.3
