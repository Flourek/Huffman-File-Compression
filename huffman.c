//
// Created by Flourek on 23/05/2022.
//
#include "stdlib.h"
#include "string.h"
#include "header.h"


Node * huffman_tree(const unsigned char input[], int input_s) {

    Node *set = calloc(300, sizeof(Node));

    // populate that shit with probabilites and values
    for (int i=0; i < input_s; ++i){
        set[input[i]].value = input[i];
        set[input[i]].prob++;
    }


    // sort :pape:
    qsort(set, 256, sizeof(Node), qsort_asc);


    // calculate length and get rid of 0 value nodes
    int len_s = 0;
    while(set[len_s].prob) len_s++;
    set = realloc(set, len_s * sizeof(Node));


//    for (int i = 0; i < len_s; ++i) {
//        printf("%c%d ", set[i].value, set[i].prob);
//    }
//    printf("\n");


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


unsigned char ** generate_codes(Node *s, char *code, unsigned char **codes) {
    if(!s) return 0;

    // each recursion needs it's own copy to keep the previous ones intact
    char *new_code = calloc(256,1);
    memcpy(new_code, code, 256);
    if(s->code)
        strcat(new_code, &s->code);

    if(s->left)
        codes = generate_codes(s->left, new_code, codes);
    if(s->right)
        codes = generate_codes(s->right, new_code, codes);

    // save the values, runs at the end of every branch
    if(!s->right && !s->left)
    {
        codes[s->value] = new_code;
//        printf("%c %s ", s->value, new_code);
    }

    return codes;
}

