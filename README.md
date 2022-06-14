# Huffman-File-Compression

Simple program that uses Huffman Coding to compress any type of file. It uses canonical Huffman codes to efficiently reconstruct the tree.

> ![Screenshot](images/screenshot.png?raw=true "Title")

# EPI File specification
```c
    char extension[8];
    int codes_map_len;
    CodeBlock *codes_map;
    int last_block_offset;
    DataBlock *data_blocks;
```

# Requirements
> CMAKE 3.16.3
