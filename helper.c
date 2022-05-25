//
// Created by Flourek on 23/05/2022.
//
#include "string.h"
#include "stdlib.h"
#include "header.h"
#include "sys/stat.h"

int qsort_asc(const void *p1, const void *p2) {
    Node *node1 = (Node*)p1;
    Node *node2 = (Node*)p2;
    return (- node1->prob + node2->prob);
}


char * int_to_binary_str(int x) {

    char *binary_str = calloc(32 + 1,1);
    int k;

    for (int i = 1; i <= 32; i++) {
        k = x >> -i;
        if (k & 1)
            strcat(binary_str, "1");
        else
            strcat(binary_str, "0");
    }

    return binary_str;
}


FILE * create_file(char path[]){
    struct stat buffer;
    int status = stat(path, &buffer);

    char extension[99];
    strcpy(extension,  strrchr(path, '.'));


    while (!status){   // File exists
        char *p_extension = strrchr(path, '.');
        strcpy(p_extension, " - Copy");
        strcat(path, extension);

        status = stat(path, &buffer);
    }

    return fopen(path, "wb");
}
