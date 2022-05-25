//
// Created by Flourek on 23/05/2022.
//

#ifndef ZIPPROJECT_HEADER_H
#define ZIPPROJECT_HEADER_H

#include "stdio.h"
#include "stdint.h"

#define BLOCK_SIZE sizeof(uint32_t) * 8

typedef struct Key {
    char code[256];
    unsigned char value;
} Key;


typedef struct Block {
    uint16_t code;
    uint8_t offset;
    uint8_t symbol;
} Block;

typedef struct Node {
    struct Node *left;
    struct Node *right;
    int prob;
    unsigned char value;
    char code;
} Node;



int qsort_asc(const void *p1, const void *p2);

FILE * create_file(char path[]);

char * int_to_binary_str(int x);

unsigned char ** generate_codes(Node *s, char *code, unsigned char **codes);

Node * huffman_tree(const unsigned char input[], int input_s);

int zip (FILE *src_file, char dest_path[]);

int unzip(FILE * src_file, char dest_path[]);


#endif //ZIPPROJECT_HEADER_H
