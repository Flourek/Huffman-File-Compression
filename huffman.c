//
// Created by Flourek on 23/05/2022.
//
#include "stdlib.h"
#include "string.h"
#include "header.h"


Node * huffman_tree(const unsigned char input[], int input_s) {

    Node *set = calloc(300, sizeof(Node));

    // populate  with probabilites and values
    for (int i=0; i < input_s; ++i){
        set[input[i]].value = input[i];
        set[input[i]].prob++;
    }

    qsort(set, 256, sizeof(Node), qsort_asc);


    // calculate length and get rid of 0 value nodes
    int len_s = 0;
    while(set[len_s].prob) len_s++;
    set = realloc(set, len_s * sizeof(Node));


    while (len_s >= 2) {
        Node new_node = {0};
        new_node.prob = set[len_s - 1].prob + set[len_s - 2].prob;
        new_node.left = &set[len_s - 2];
        new_node.right = &set[len_s - 1];
        new_node.left->code = '0';
        new_node.right->code = '1';
        new_node.value = 0;

        // reduce the size of the set nondestructively
        len_s--;
        Node *new_set = calloc(len_s, sizeof(Node));
        memmove(new_set, set, (len_s - 1) * sizeof(Node)); // keep "new_node.left" in memory

        new_set[len_s - 1] = new_node;
        set = new_set;

        // use binary search instead
        qsort(set, len_s, sizeof(Node), qsort_asc);
    }

    return &set[0];
}


void generate_canonical_codes(CodeMap *map){
    int code = 0;
    for (int i = 0; i < map->len; ++i) {
        strncpy(map->codes[i].code, int_to_binary_str(code) + 32 - map->codes[i].len, MAX_CODE_LEN);
        code = (code + 1) << (map->codes[i + 1].len - map->codes[i].len);
    }
}

CodeMap * huffmanify(unsigned char *input, int src_size) {

    // build a huffman tree out of the data_str and create a map of codes, where a symbol is the key
    Node *head = huffman_tree(input, src_size);
    char *empty = calloc(256,1);
    char **codes_map = calloc(256, 256);
    codes_map = generate_huffman_codes(head, empty, codes_map);

    CodeMap * map = calloc(1, sizeof(CodeMap));

    // get the number of existing codes
    for (int i = 0; i < 256; ++i)
        if(codes_map[i]) map->len++;

    map->codes  = calloc(map->len, sizeof(CanonCode));

    for (int i = 0, j = 0; i < 256; ++i)
        if (codes_map[i]) {
            map->codes[j].symbol = i;
            map->codes[j].len = strlen(codes_map[i]);
            strncpy(map->codes[j].code, codes_map[i], 256);
            j++;
        }

    qsort(map->codes, map->len, sizeof(CanonCode), qsort_canon_alphabetically);
    qsort(map->codes, map->len, sizeof(CanonCode), qsort_canon_codelen);

    generate_canonical_codes(map);

    return map;
}


char ** generate_huffman_codes(Node *s, char *code, char **codes) {
    if(!s) return 0;

    // each recursion needs it's own copy to keep the previous ones intact
    char *new_code = calloc(256,1);
    memcpy(new_code, code, 256);
    if(s->code)
        strcat(new_code, &s->code);

    if(s->left)
        codes = generate_huffman_codes(s->left, new_code, codes);
    if(s->right)
        codes = generate_huffman_codes(s->right, new_code, codes);

    // save the values, runs at the end of every branch
    if(!s->right && !s->left)
    {
        codes[s->value] = new_code;
//        printf("%c %s ", s->value, new_code);
    }

    return codes;
}

