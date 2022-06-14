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

int qsort_canon_alphabetically(const void *p1, const void *p2){
    CanonCode *x = (CanonCode*) p1;
    CanonCode *y = (CanonCode*) p2;
    return (- x->symbol + y->symbol);
}

int qsort_canon_codelen(const void *p1, const void *p2){
    CanonCode *x = (CanonCode*) p1;
    CanonCode *y = (CanonCode*) p2;
    return (x->len - y->len);
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

int get_file_size(FILE * f){
    fseek(f, 0, SEEK_END);
    return ftell(f);
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

int pearson8(char *str){
    static const uint8_t T[256] = {136, 209, 83, 131, 28, 10, 200, 82, 4, 38, 223, 162, 35, 22, 171, 33, 63, 66, 235, 229, 156, 71, 1, 49, 133, 137, 195, 154, 91, 78, 237, 96, 64, 45, 183, 161, 160, 32, 69, 181, 216, 95, 218, 176, 53, 118, 246, 51, 139, 101, 169, 104, 214, 228, 50, 179, 98, 73, 212, 111, 93, 18, 210, 151, 239, 72, 165, 108, 20, 177, 224, 6, 135, 76, 120, 17, 219, 99, 97, 107, 231, 39, 60, 119, 140, 150, 84, 58, 190, 167, 21, 251, 145, 141, 92, 19, 102, 201, 42, 192, 245, 12, 172, 23, 43, 36, 116, 109, 13, 247, 242, 26, 14, 130, 152, 0, 128, 114, 75, 9, 153, 25, 241, 80, 252, 62, 11, 46, 193, 16, 187, 56, 142, 129, 182, 44, 113, 202, 143, 226, 205, 54, 203, 106, 48, 222, 52, 236, 2, 197, 178, 225, 254, 94, 248, 206, 227, 8, 170, 207, 34, 189, 217, 204, 158, 30, 194, 67, 74, 57, 191, 213, 215, 27, 125, 3, 198, 24, 164, 168, 211, 86, 132, 115, 89, 233, 79, 186, 250, 174, 85, 243, 148, 121, 61, 90, 29, 126, 240, 166, 65, 7, 238, 208, 87, 103, 41, 81, 159, 112, 15, 175, 157, 31, 184, 68, 105, 196, 185, 146, 234, 199, 138, 122, 100, 123, 180, 134, 40, 244, 230, 173, 149, 220, 147, 221, 253, 55, 110, 163, 117, 188, 144, 232, 155, 37, 70, 59, 88, 77, 127, 5, 124, 255, 249, 47};
    int len = strlen(str);
    int hash = 0;

    for (int i = 0; i < len; ++i) {
        hash = T[hash ^ str[i]];
    }

    return hash;
}

