//
// Created by Flourek on 23/05/2022.
//

#ifndef ZIPPROJECT_HEADER_H
#define ZIPPROJECT_HEADER_H

#include "stdio.h"
#include "stdint.h"

#define MAX_CODE_LEN 12
#define KB(x) ((size_t) (x) << 10)
#define PATH_MAX 260

typedef uint32_t DataBlock;
#define DATA_BLOCK_SIZE (sizeof(DataBlock) * 8)

typedef struct CodeBlock {
    unsigned code: 10;
    unsigned offset: 5;
    unsigned symbol: 8;
} __attribute__((packed)) CodeBlock;


typedef struct EpiFile {
    char extension[8];
    int codes_map_len;
    CodeBlock *codes_map;
    int last_block_offset;
    DataBlock *data_blocks;
} EpiFile;


typedef struct Node {
    struct Node *left;
    struct Node *right;
    int prob;
    unsigned char value;
    char code;
} Node;


typedef struct Key {
    char code[256];
    unsigned char value;
} Key;


int get_file_size(FILE * f);

int qsort_asc(const void *p1, const void *p2);

FILE * create_file(char path[]);

char * int_to_binary_str(int x);

unsigned char ** generate_codes(Node *s, char *code, unsigned char **codes);

Node * huffman_tree(const unsigned char input[], int input_s);

void zip (FILE *src_file, char dest_path[]);

void unzip(FILE * src_file, char dest_path[]);


#endif //ZIPPROJECT_HEADER_H
