# Huffman-File-Compression

Simple program that uses Huffman Coding to compress any type of file. It uses canonical Huffman codes to efficiently reconstruct the tree.

> ![Screenshot](images/screenshot.png?raw=true "Title")

# EPI File specification
```c
    char extension[8];       // the first 8 bytes store the extension of the original file 
    int codes_map_len;       // stores how many "CodeBlocks" to read   
    CodeBlock *codes_map;    // stores the huffman codes as SYMBOL | CODE LENGTH to reconstruct the tree
    int last_block_offset;   // the last "DataBlock" 
    DataBlock *data_blocks;  // packs the otherwise string of binary codes into a 32 bit integer  
```

# Requirements
> CMAKE 3.16.3
